#include<bits/stdc++.h>
using namespace std;

// Forward declarations of classes to avoid compilation errors.
class Issue;
class Agent;
class CustomerIssueResolutionSystem;

// Class representing a customer issue.
class Issue {
public:
	static int issueCount;
    string issueId;
    string transactionId;
    string issueType;
    string subject;
    string description;
    string email;
    string status;
    string resolution;
    
    Issue(string tId, string iType, string sub, string des, string cusEmail){
    	issueId = "I" + to_string(++issueCount);
    	transactionId = tId;
    	issueType = iType;
    	subject = sub;
    	description = des;
    	email = cusEmail;
    	status = "Open";
    	resolution = "";
    }
};

int Issue::issueCount = 0; // Initialize static issueCount to 0.

// Class representing an agent who handles customer issues.
class Agent {
public:
    string agentId;
    string agentEmail;
    string agentName;
    vector<string> expertise;
    queue<Issue*> issueQueue;
    vector<Issue*> workHistory;
    bool isAvailable;

    Agent(string email, string name, vector<string> exp) {
        agentId = "A" + email;
        agentEmail = email;
        agentName = name;
        expertise = exp;
        isAvailable = true;
    }
    
	// Method to assign an issue to the agent.
    void assignIssue(Issue* issue){
        issueQueue.push(issue);
        isAvailable = false;
    }
    
	// Method to resolve the current issue.
    void resolveCurrentIssue(string resolution){
        if(!issueQueue.empty()){
            Issue* currIssue = issueQueue.front();
            currIssue->status = "Resolved";
            currIssue->resolution = resolution;
            workHistory.push_back(currIssue);
            issueQueue.pop();
            if(issueQueue.empty()){
                isAvailable = true;
            }
        }
    }
    
	// Method to mark issue status
    void markIssueInProgres(Issue* issue, string status, string resolution){
        issue->status = status;
        issue->resolution = resolution;
    }
    
	// Method to display the agent's work history.
    void displayWorkHistory() {
        cout << agentName << " -> ";
        for(auto& issue : workHistory){
            cout << issue->issueId << " ";
        }
        cout << endl;
    }
};

// Class representing the customer issue resolution system.
class CustomerIssueResolutionSystem {
private :
    unordered_map<string, Issue*> issues;
    unordered_map<string, Agent*> agents;
    queue<Issue*> pendingIssues;
    
public:
	// Method to create a new issue.
    void createIssue(string transactionId, string issueType, string subject, string description, string email) {
        Issue* issue = new Issue(transactionId, issueType, subject, description, email);
        issues[issue->issueId] = issue;
        cout << "Issue " << issue->issueId << " created against transaction " << transactionId << endl;
    }
    
	// Method to add a new agent.
    void addAgent(string agentEmail, string agentName, vector<string> expertise) {
        Agent* agent = new Agent(agentEmail, agentName, expertise);
        agents[agent->agentId] = agent;
        cout << "Agent " << agent->agentId << " created" << endl;
    }
    
	// Method to assign an issue to an available agent.
    void assignIssue(string issueId) {
        if (issues.find(issueId) == issues.end()) {
            cout << "Issue not found!" << endl;
            return;
        }

        Issue* issue = issues[issueId];

        // Find an available agent with the required expertise
        for (auto& agentEntry : agents) {
            Agent* agent = agentEntry.second;

            // Use find to check if the agent has the required expertise
            auto it = find(agent->expertise.begin(), agent->expertise.end(), issue->issueType);
            if (agent->isAvailable && it != agent->expertise.end()) {
                agent->assignIssue(issue);
                cout << "Issue " << issueId << " assigned to agent " << agent->agentId << endl;
                return;
            }
        }

        // No free agent found, add the issue to the pending queue
        pendingIssues.push(issue);
        cout << "Issue " << issueId << " added to waitlist" << endl;
    }

    // Get issues based on filter (email or type)
    void getIssues(string filter, string value) {
        for (auto& issueEntry : issues) {
            Issue* issue = issueEntry.second;
            if ((filter == "email" && issue->email == value) ||
                (filter == "type" && issue->issueType == value)) {
                cout << issue->issueId << " {" << issue->transactionId << ", " << issue->issueType << ", " 
                     << issue->subject << ", " << issue->description << ", " << issue->email << ", " 
                     << issue->status << "}" << endl;
            }
        }
    }

    // Update issue status
    void updateIssue(string issueId, string status, string resolution) {
        if (issues.find(issueId) == issues.end()) {
            cout << "Issue not found!" << endl;
            return;
        }

        Issue* issue = issues[issueId];
        issue->status = status;
        issue->resolution = resolution;
        cout << "Issue " << issueId << " status updated to " << status << endl;
    }

    // Resolve issue
    void resolveIssue(string issueId, string resolution) {
        for (auto& agentEntry : agents) {
            Agent* agent = agentEntry.second;
            if (!agent->issueQueue.empty() && agent->issueQueue.front()->issueId == issueId) {
                agent->resolveCurrentIssue(resolution);
                cout << "Issue " << issueId << " marked resolved" << endl;

                // Assign the next pending issue if any
                if (!pendingIssues.empty()) {
                    Issue* nextIssue = pendingIssues.front();
                    pendingIssues.pop();
                    agent->assignIssue(nextIssue);
                    cout << "Issue " << nextIssue->issueId << " assigned to agent " << agent->agentId << endl;
                }
                return;
            }
        }

        cout << "Issue not found in any agent's queue!" << endl;
    }

    // View agent's work history
    void viewAgentsWorkHistory() {
        for (auto& agentEntry : agents) {
            Agent* agent = agentEntry.second;
            agent->displayWorkHistory();
        }
    }
};


int main() {
	// your code goes here
	
	CustomerIssueResolutionSystem system;

    system.createIssue("T1", "Payment Related", "Payment Failed", "My payment failed but money is debited", "testUser1@test.com");
    system.createIssue("T2", "Mutual Fund Related", "Purchase Failed", "Unable to purchase Mutual Fund", "testUser2@test.com");
    system.createIssue("T3", "Payment Related", "Payment Failed", "My payment failed but money is debited", "testUser2@test.com");

    system.addAgent("agent1@test.com", "Agent 1", {"Payment Related", "Gold Related"});
    system.addAgent("agent2@test.com", "Agent 2", {"Payment Related"});

    system.assignIssue("I1");
    system.assignIssue("I2");
    system.assignIssue("I3");

    system.getIssues("email", "testUser2@test.com");
    system.getIssues("type", "Payment Related");

    system.updateIssue("I3", "In Progress", "Waiting for payment confirmation");
    system.resolveIssue("I3", "PaymentFailed debited amount will get reversed");

    system.viewAgentsWorkHistory();
	return 0;
}