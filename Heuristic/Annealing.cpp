#include<bits/stdc++.h>
template<class S,long long(*score)(S),S(*change)(S)>
struct Annealing{
  S first_S;
  double cooling_rate;
  double cool = 1;
  double cur_score;
  S cur_S;
  double best_score;
  S best_S;

  Annealing(S fs,double cr) : first_S(fs),cooling_rate(cr){
    cur_S = fs;
    best_S = fs;
    cur_score = score(fs);
    best_score = cur_score;
    std::srand((unsigned int)std::time);
  }
  void Anneal(){
    cool *= cooling_rate;
    double r2 = (double)std::rand()/(double)RAND_MAX;
    S new_S = change(cur_S);
    double new_score = score(new_S);
    double score_d = new_score-cur_score;
    if(score_d>0||r2<std::exp(score_d/cool)){
        cur_S = new_S;
        cur_score = new_score;
    }
    if(new_score>best_score){
        best_S = new_S;
        best_score = new_score;
    }
  }
};