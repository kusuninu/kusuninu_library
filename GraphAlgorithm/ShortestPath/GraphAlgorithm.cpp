#include<bits/stdc++.h>

std::vector<long long> Dijkstra(const std::vector<std::vector<std::pair<int,long long>>>& G){
	int s = 0;
	const long long INF = 1LL << 61;
	int N = G.size();
	std::vector<long long> dis(N,INF);
	dis[s] = 0;
	std::priority_queue<std::pair<long long,int>,std::vector<std::pair<long long,int>>,std::greater<std::pair<long long,int>>> pq;
	pq.push({0,s});
	while(!pq.empty()){
		auto p = pq.top();
		pq.pop();
		long long d = p.first;
		int n = p.second;
		if(dis[n] < d) continue;
		for(auto& e : G[n]){
			if(d+e.second < dis[e.first]){
				dis[e.first] = d+e.second;
				pq.emplace(d+e.second,e.first);
			}
		}
	}
	return dis;
}

std::vector<long long> Dijkstra(const std::vector<std::vector<std::pair<int,long long>>>& G,int s){
	const long long INF = 1LL << 61;
	int N = G.size();
	std::vector<long long> dis(N,INF);
	dis[s] = 0;
	std::priority_queue<std::pair<long long,int>,std::vector<std::pair<long long,int>>,std::greater<std::pair<long long,int>>> pq;
	pq.push({0,s});
	while(!pq.empty()){
		auto p = pq.top();
		pq.pop();
		long long d = p.first;
		int n = p.second;
		if(dis[n] < d) continue;
		for(auto& e : G[n]){
			if(d+e.second < dis[e.first]){
				dis[e.first] = d+e.second;
				pq.emplace(d+e.second,e.first);
			}
		}
	}
	return dis;
}


//negative loop -> true
bool Bellman_Ford(const std::vector<std::vector<std::pair<int,long long>>>& G,std::vector<long long>& dis){
	assert(G.size()==dis.size());
	const long long INF = 1LL<<61;
	int N = G.size();
	for(int i = 0;i < N;i++){
		bool finished = true;
		for(int j = 0;j < N;j++){
			for(auto [t,c] : G[j]){
				if(dis[j]!=INF&&dis[j]+c<dis[t]){
					dis[t] = dis[j]+c;
					finished = false;
				}
			}
		}
		if(i==N-1&&!finished) return true;
		if(finished) break;
	}
	return false;
}

void Warshall_Floyd(std::vector<std::vector<long long>> &dis) {
    int V = dis.size();
    for (int k = 0; k < V; k++) {
        for (int i = 0; i < V; i++) {
            for (int j = 0; j < V; j++) {
                dis[i][j] = std::min(dis[i][j], dis[i][k] + dis[k][j]);
            }
        }
    }
}
