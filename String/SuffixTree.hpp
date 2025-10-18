#include <bits/stdc++.h>
/*
    Copyright © (c) okoteiyu 2024. 
    Released under the MIT license(https://opensource.org/licenses/mit-license.php) 
                   

    - CharType 型の列 S に対して頂点 0 を根とする Suffix Tree を構築する。添字は基本的に 0-index。
        |- 列は string / vector の両方に対応。
            |- CharType は char からキャストされることができる必要があるものとする。
        |- Suffix Tree 上の各位置は S の連続部分列と 1 対 1 対応しており、実質的に連続部分列を重複なく列挙している。
        |- ノードの番号は生成順 (root の id が 0)。
        |- 空列も接尾辞として含み、辞書順で最小とする(suffix_id は |S| とする)
            |- 空列である接尾辞に対応するノードが root であるとする。
    |
    - Suffix Tree のデータのハンドルとして、Node と TreePlace クラスを内部に定義。詳細はクラスの中身を参照。
    |
    - Suffix Tree 構築
        |- push_back(s_) : S の末尾に列または文字 s_ をくっつけて Suffix Tree にも反映させる。
        |- determine() : 構築を終了する。
        |- build(s) := 初期状態から push_back(s) + determine()
    |
    - determine によって構築が終了することで、多くの機能が解放される。
        |- determine する前に使える機能もあるが少ない。
        |- determine する前に使える機能の説明の後には [(*)] を書いておく。
    |
    - 探索メソッド
        |- find(t) := t が S の連続部分列かどうかを判定。(*)
        |- count(t) := t が S の連続部分列として何回現れるか計算。
        |- search(t):= t を検索し、移動できた位置までのデータを返す。(*)
        |- search(l,r) := S の区間 [l,r) に対応する位置データを返す。ただし、refer は非明示的。
        |- decode_suffix(TreePlace x) := 位置 x に対応する列を連続部分列として含む接尾辞の開始 index のうちの 1 つを返す。(*)
            |- 返り値を l とすると、位置 x は S の [l,l + |x|) に対応する (|x| は位置 x に対応する列の長さ)
    |
    - 木構造メソッド
        |- Node クラスと TreePlace クラスは private なので、インターフェースを通して取得する。
            |- get_root() : 根ノードのポインタを返す。(*)
            |- from_root(bool refering = true) : 根に対応する位置を返す(位置データの refering を指定する)。(*)
            |- get_node(i) : 頂点番号が i のノードのポインタを返す。
        |- get_suffix_node(i) : S[i] から始まる接尾辞に対応するノードのポインタを返す。
        |- get_depth_as_tree(i) := root から頂点 i までの辺の本数を返す。
        |- edges() := Suffix Tree の辺集合を返す。(*)
        |- dfs(f) := root から dfs するときの到達順に頂点のポインタを並べて返す(f が true なら辞書順最小優先で dfs)。(*)
        |- LCA(位置のデータ 2 つ) := 位置(辺の途中含む)の LCA のデータを返す。
    |
    - その他機能
        |- クラス外で Suffix Tree 上の移動をする際は以下を使う
            |- findValidPath(p,c) := 位置から c の方向に移動できるかどうか。(*)
            |- walkdown(p,c) := 位置 p から c の方向に移動し、p を書き換える。(*)
        |- size() := S のサイズを返す。(*)
        |- [i] : S[i] を read-only でアクセス。(*)
        |- suffix_array() : Suffix Array を返す。
        |- get_Kth_subStr(K) : 列 S の 辞書順で K 番目の連続部分列。
    - 注意 
        |- release_resource を false にしておくと、リソースの解放がされない分高速になる。
        |- データアクセスは const Node のポインタを返すので、auto で受け取ると楽
*/
template<typename CharType, bool release_resource = true , typename = std::enable_if_t<std::is_convertible_v<char, CharType>>>
class SuffixTree{
    /*   
        Copyright © (c) okoteiyu 2024. 
        Released under the MIT license(https://opensource.org/licenses/mit-license.php) 
    */
    public:

    struct Node;
    struct TreePlace;
    using Edge = std::pair<CharType,Node*>; // {ラベルの先頭の文字, 終端のノード}
    using EdgeLabel = std::pair<int,int*>; // 半開区間による部分文字列の参照

    /* 頂点データ */
    struct Node{
        public:
        // 頂点番号
        const int id = -1;
        // [x] := 要素 x で続く辺の先のノード
        std::unordered_map<CharType , Node*> child;
        // 親ノード
        Node* parent = nullptr;
        // このノードまでのラベルからなる文字列の先頭を取り除いた文字列に対応する頂点へのリンク
        Node* SuffixLink = nullptr;
        // 自身に入る辺
        EdgeLabel label = std::make_pair(-1, nullptr);
        // このノードに対応する列の長さ (葉ノードについては不定なので、内部ノードにのみ定義)
        int path_label_length_sub = 0; 
        // 終端ノードなら何文字目から始まる接尾辞か(0-index)。そうでないなら -1。
        int suffix_id = -1; 
        // 自分以降の部分木が持つ終端ノードの個数
        int SubTreeEndPointCount = 0;      
        // このノードから移動して得られる文字列の種類数 (空文字列はカウントしない)
        long long SubTreeKindOfPrefix = 0; 
        // 葉頂点かどうか
        bool is_leaf = true;

        // ラベル、頂点番号、親頂点
        Node(EdgeLabel label_ , int id_, Node* parent_ , bool is_leaf_) 
            : id(id_), parent(parent_), label(label_), is_leaf(is_leaf_) {}

        Node()=default;
        Node(const Node&)=default;
        Node(Node&&)=default;
        Node& operator=(Node&&)=default;
        Node& operator=(const Node&)=default;

        // 辺に書かれた文字列の半開区間の左端 (0-index)
        const int cleft() const noexcept {
            return label.first;
        }
        // 辺に書かれた文字列の半開区間の右端 (0-index)
        const int cright() const noexcept {
            return (*label.second);
        }
        // 入る辺の長さ
        const int label_length() const noexcept {
            if(this->parent == nullptr)return 0;// root の場合は 0 とする
            return cright() - cleft();
        }
        // 根からノードまでの辺の文字数
        const int path_label_length() const noexcept {
            if(this->parent == nullptr)return 0;
            return this->parent->path_label_length_sub + label_length();
        }
        // このノードが終端ノードかどうか
        const bool isEndPoint() const noexcept {
            return (this->suffix_id >= 0);
        }
        // このノードが根かどうか
        const bool is_root() const noexcept {
            return bool(this->parent == nullptr);
        }
    };


    /*
        - (from, to, place) := 頂点 from から、to の方向に place 進んだ位置を指すデータ。
        - ちょうど 頂点 x の上である場合、(x, nullptr, 0) で統一する。
        - この位置に対応する列として refer を持つ。refering = false を指定すると refer を非明示的にして定数サイズオブジェクトとして扱える。
    */
    struct TreePlace{
        Node* from = nullptr;
        Node* to = nullptr;
        int place = 0;

        TreePlace()=default;
        // シャローコピー
        TreePlace(const TreePlace&)=default;
        TreePlace(TreePlace&&)=default;
        TreePlace& operator=(TreePlace&&)=default;
        TreePlace& operator=(const TreePlace&)=default;

        // 根から現在地までのパス上の文字数 (距離)
        int path_label_length() const noexcept {
            return this->from->path_label_length() + place;
        }
        // 根の位置かどうか
        bool is_root() const noexcept {
            return bool(this->path_label_length() == 0);
        }

        // この位置と対応する列を明示的に管理するかどうか
        bool refering = true;
        // この位置に対応する列 (pop_front するので deque で管理)
        std::deque<CharType> refer;
        
        // 現在地から、先頭要素を削除した位置に移動(明示的に対応する列 refer を管理する場合)。
        bool walkSuffixLink(){
            assert(refering);
            assert(refer.size() == this->path_label_length());
            if(this->is_root())return false; // これ以上 Suffix Link を辿れないなら false
            refer.pop_front();// 先頭の文字を削除する
            this->from = this->from->SuffixLink;// Suffix Link を辿って移動する
            // 辺上の位置を復元 (先頭を削除した refer の長さ -  直近の頂点への長さ)
            this->place = refer.size() - this->from->path_label_length();
            // 分岐点にいるなら終了して OK (place で判定する必要性に注意)
            if(this->place == 0){
                this->to = nullptr;
                return true;
            }

            // Suffix Link を辿ったあとの to を、from の深さと refer から復元
            this->to = this->from->child[refer[this->from->path_label_length()]];
            // place が from と to の間にくるように整形
            while(this->to != nullptr && this->to->label_length() <= this->place){
                this->place -= this->to->label_length(); 
                this->from = this->to;
                this->to = nullptr;
                if(this->place > 0)this->to = this->from->child[refer[this->from->path_label_length()]];
            }
            return true;
        }

        // refer が非明示な状況で Suffix Link を辿って移動する場合。
        // TreePlace は何らかの列の連続部分列なので、列 S_ と S_ のどこから始まる部分文字列かを表す整数を引数に取る。
        template<class ArrayType, typename = std::enable_if_t<
            std::is_same_v<typename std::decay<ArrayType>::type, std::string>
            || std::is_same_v<typename std::decay<ArrayType>::type, std::vector<CharType>>>>
        bool walkSuffixLink(const ArrayType& S_ , int start_index){
            refering = false;
            refer.clear();
            if(this->is_root())return false; 
            int refer_size = this->path_label_length()-1;// refer の先頭を削除したものの長さ
            start_index += 1;
            this->from = this->from->SuffixLink;
            this->place = refer_size - this->from->path_label_length();
            if(this->place == 0){
                this->to = nullptr;
                return true;
            }
            this->to = this->from->child[S_.at(start_index + this->from->path_label_length())];
            while(this->to != nullptr && this->to->label_length() <= this->place){
                this->place -= this->to->label_length(); 
                this->from = this->to;
                this->to = nullptr;
                if(this->place > 0)this->to = this->from->child[S_[start_index + this->from->path_label_length()]];
            }
            
            return true;
        }

        // 位置が同じ位置かどうか (refer を参照しないので O(1) 時間)
        friend bool operator==(const TreePlace& a,  const TreePlace& b){
            return (a.from == b.from && a.to == b.to && a.place == b.place);
        }
        
        // 出力
        friend std::ostream& operator<<(std::ostream& os , const TreePlace& p){
            os << "(" << p.from->id << ", ";
            if(p.to == nullptr)os << "?";
            else os << p.to;
            os << ", " << p.place << ")";
            return os;
        }
    };
    

    /* ==============================================================================================
    ** メンバ(これらは内部関数でのみ編集される) ============================================================
    ** ============================================================================================*/

    private:

    // エラー終了
    void build_error(){
        std::cerr << "Don't push_back to determined SuffixTree." << std::endl;
        exit(1);
    } 
    void calc_error(){
        std::cerr << "Cannot Calc from Not-Determined SuffixTree." << std::endl;
        exit(1);
    } 
    

    // 辺の順序(<)比較用のラムダ
    const std::function<bool(Edge,Edge)> compare_edge = [](Edge a ,Edge b){
        return a.first < b.first;
    };

    // 頂点番号割り振り用のカウンタ
    int m_vertex_id;
    // Suffix Tree 構築元の列
    std::vector<CharType> S;
    // S の終端を表すためのシンボル
    int m_EndS;
    // 根を表すデータ
    Node* root = nullptr;
    // 最後に追加された葉ノード
    Node* lastCreatedLeafNode = nullptr;
    // ノードを追加する際に、共有する部分の(端点の)位置データ
    TreePlace m_build_front;
    // [x] := 頂点番号 x のノードへのポインタ
    std::vector<Node*> m_Nodes;
    // [x] := x 文字目から始まる Suffix に対応するノードへのポインタ (0-index)
    std::vector<Node*> m_SuffixNodes;
    // [x] := 根から頂点 x まで辺が何本か
    std::vector<int> m_tree_depth;
    // [i][u] := 頂点 u の 2^i 個上の頂点
    std::vector<std::vector<int>> m_parent; 
    /*== 重複して初期化を行わないためのフラグ ==*/
    // 構築が完全に終了したか。
    bool m_determined = false;
    // 基本的なデータを構築したか
    bool m_initialized_basic_data = false;
    // 部分木の集約データを構築したか
    bool m_initialized_tree_dp = false;
    // LCA のデータを構築したかどうか
    bool m_initialized_LCA = false;


    
    // メンバの初期化
    void initialize(){
        S.clear();
        m_Nodes.clear();
        m_SuffixNodes.clear();
        m_parent.clear();
        m_tree_depth.clear();
        // 構築が終了したか
        m_determined = false;
        // 基本データを構築したか
        m_initialized_basic_data = false;
        // 部分木の集約データを構築したか
        m_initialized_tree_dp = false;
        // LCA データを構築したか
        m_initialized_LCA = false;
        // 構築用データ初期化
        m_vertex_id = 0;
        // 終端を表すシンボル
        m_EndS = 0;
        // 根頂点
        root = new Node({0,new int(0)}, m_vertex_id++, nullptr, false);
        // root からは root にリンクが出ている (共通化のためここだけ例外)
        root->SuffixLink = root; 
        // 最後に追加された葉ノード
        lastCreatedLeafNode = nullptr;
        // 位置データは refer を明示的に持つ
        m_build_front = from_root(true);
    }


    // 基本的なデータの構築(ノードへのアクセス用の配列など)
    void init_basic_data(){
        if(root == nullptr)return;
        if(m_initialized_basic_data)return;
        if(!m_determined)calc_error();
        
        m_initialized_basic_data = true;
        m_Nodes.resize(tree_size() , nullptr);
        m_tree_depth.resize(tree_size() , 0);
        m_SuffixNodes.resize(S.size()+1 , nullptr);// 空文字列 (|S| からの接尾辞) も含む
        std::vector<Node*> s = dfs();
        m_tree_depth[root->id] = 0;
        // DFS 順に実行
        for(Node* now : s){
            // id と Node* を対応付ける
            m_Nodes[now->id] = now;
            // x 文字目から始まる Suffix に対応するノードを管理
            if(now->isEndPoint())m_SuffixNodes[now->suffix_id] = now; 
            // 木としての深さを計算
            if(now->parent != nullptr)m_tree_depth[now->id] = m_tree_depth[now->parent->id] + 1;
        }
    }


    // 部分木の集約データの構築
    void init_tree_dp(){
        if(root == nullptr)return;
        if(m_initialized_tree_dp)return;
        if(!m_determined)calc_error();

        if(this->size() == 0){}
        m_initialized_tree_dp = true;
        std::vector<Node*> rdfs = dfs();
        reverse(rdfs.begin() , rdfs.end());
        for(Node* now : rdfs){   
            now->SubTreeKindOfPrefix = 0;
            now->SubTreeEndPointCount = 0;
        }
        for(Node* now : rdfs){
            if(now->isEndPoint())now->SubTreeEndPointCount+=1;
            if(now->parent == nullptr)continue;
            // 部分木のノードのうち、接尾辞と対応しているものの個数
            now->parent->SubTreeEndPointCount += now->SubTreeEndPointCount; 
            // 部分木のパスが表す文字列の種類数 + 辺の分
            now->parent->SubTreeKindOfPrefix += now->SubTreeKindOfPrefix + now->label_length();
        }
    }


    // LCA 用のデータの構築
    void init_LCA(){
        if(root == nullptr)return;
        if(m_initialized_LCA)return;
        if(!m_determined)calc_error();

        m_initialized_LCA = true;
        const int K = log2(tree_size())+4;
        m_parent.resize(K,vector<int>(tree_size(),-1));
        std::vector<Node*> s = dfs();        
        for(Node* now : s){
            if(now->parent == nullptr)continue;
            m_parent[0][now->id] = now->parent->id;
        }
        for(int x = 1 ; x < K ; x++){
            for(int u = 0 ; u < m_parent[x].size(); u++){
                if(m_parent[x-1][u] < 0)continue;
                m_parent[x][u] = m_parent[x-1][m_parent[x-1][u]];
            }
        }
    }


    // x をオンラインで S の末尾に連結する 
    void push_back_sub(CharType x){
        // 初回は必要データの構築を自動で行う
        if(root == nullptr || S.size() == 0)initialize();
        if(m_determined)build_error();
        
        S.push_back(x); 
        // 終端を表すシンボルの実体を更新
        m_EndS++;
        Node* lastUsedBranch = nullptr;
        do{
            // m_build_front → x に移動できるなら x の方向に移動して終了
            if(findValidPath(m_build_front,x)){
                if(lastUsedBranch!=nullptr)lastUsedBranch->SuffixLink = m_build_front.from; 
                walkdown(m_build_front,x);
                break;
            }
            // Branch を分岐点として S の接尾辞 {m_build_front + x} を Suffix Tree に挿入する
            Node* Branch = m_build_front.from;
            if(m_build_front.to != nullptr){// 辺の途中に分岐点を新たに作成する場合
                int middle = m_build_front.to->cleft() + m_build_front.place;
                Branch = new Node({m_build_front.to->cleft() , new int(middle) } ,m_vertex_id++ , m_build_front.from, false);
                // 辺の上に新たな Branch ができたことによる辺の張り替えなど
                m_build_front.from->child[ S[m_build_front.to->cleft()]] = Branch;
                Branch->parent = m_build_front.from;
                Branch->path_label_length_sub = m_build_front.path_label_length();
                m_build_front.to->label.first = middle;
                Branch->child[ S[m_build_front.to->cleft()] ] = m_build_front.to;
                m_build_front.to->parent = Branch;
            }
            // はみ出た部分 (1 文字分) を Leaf Node として追加する
            Node* NewLeaf = new Node( { m_EndS-1 , &m_EndS} ,m_vertex_id++ , Branch, true);
            NewLeaf->suffix_id = NewLeaf->cright() - m_build_front.path_label_length() - 1;
            Branch->child[x] = NewLeaf;

            if(lastUsedBranch!=nullptr)lastUsedBranch->SuffixLink = Branch;// 使った分岐点と leaf に Suffix Link を張る
            if(lastCreatedLeafNode != nullptr)lastCreatedLeafNode->SuffixLink = NewLeaf;
            
            lastUsedBranch = Branch;
            lastCreatedLeafNode = NewLeaf;
            // m_build_front を m_build_front + x に移動できるようになるまで、Suffix Link を辿る。
        }while(m_build_front.walkSuffixLink());
    }

    
    // 各接尾辞に対応するノードが Suffix Tree に存在するように変形する。
    void determine_sub(){
        if(m_determined)return;
        m_determined = true;
        // Suffix Link を辿りながら m_build_front の箇所にノードを生成する。
        do{
            Node* Branch = m_build_front.from;
            // 辺の途中に分岐点を新たに作成する場合
            if(m_build_front.to != nullptr){
                int middle = m_build_front.to->cleft() + m_build_front.place;
                Branch = new Node({m_build_front.to->cleft() , new int(middle) } ,m_vertex_id++ , m_build_front.from, false);
                m_build_front.from->child[ S[m_build_front.to->cleft()]] = Branch;
                Branch->parent = m_build_front.from;
                Branch->path_label_length_sub = m_build_front.path_label_length();
                m_build_front.to->label.first = middle;
                Branch->child[ S[m_build_front.to->cleft()] ] = m_build_front.to;
                m_build_front.to->parent = Branch;
            }
            // 根も空文字列に対応するものとして EndPoint になる。
            Branch->suffix_id = m_EndS - m_build_front.path_label_length();
            if(lastCreatedLeafNode!=nullptr)lastCreatedLeafNode->SuffixLink = Branch;// Suffix Link を張る
            lastCreatedLeafNode = Branch;
        }while(m_build_front.walkSuffixLink());
    }


    /* ==============================================================================================
    ** 外部向け機能 ===================================================================================
    ** ============================================================================================*/
    public:

    SuffixTree(){
        initialize();
    }

    ~SuffixTree(){
        if(release_resource)clear();
    }

    SuffixTree(const SuffixTree&) = delete;
    SuffixTree(SuffixTree&&) = delete;
    SuffixTree& operator=(const SuffixTree&) = delete;
    SuffixTree& operator=(SuffixTree&&) = delete;

    // メモリ解放
    void clear(){
        std::vector<Node*> pool = this->dfs(false);
        for(Node* x : pool)delete x;
        initialize();
    }

    // 位置データ data から要素 x に向かって進むことができるかを判定
    bool findValidPath(const TreePlace& data , CharType x) const {
        // ノードの上にいる場合は、x 方向に辺があるかを確かめる
        if(data.to == nullptr )return bool(data.from->child[x] != nullptr);
        // 辺の上にいるときは、次の文字を確かめる
        return bool(S[data.to->cleft() + data.place] == x);
    }


    // 位置データ data から x の方向に 1 進み、data を更新する
    bool walkdown(TreePlace& data , CharType x) const {
        // 進めない場合
        if(findValidPath(data,x) == false)return false;
        // 分岐点にいる場合
        if(data.to == nullptr)data.to = data.from->child[x];
        data.place++;
        // 頂点に到達した場合、from が頂点を表す様にフォーマットする
        if(data.to != nullptr && data.place == data.to->label_length()){
            data.from = data.to;
            data.to = nullptr;
            data.place = 0;
        }
        // 位置データに対応する列が明示的なら、それを更新
        if(data.refering)data.refer.push_back(x);
        return true;
    }

    // 列 V_ を Suffix Tree 構成元の列の末尾に追加する
    template<class ArrayType, typename = std::enable_if_t<
        std::is_same_v<typename std::decay<ArrayType>::type, std::string>
        || std::is_same_v<typename std::decay<ArrayType>::type, std::vector<CharType>>>>
    void push_back(const ArrayType& V_){
        for(int i = 0 ; i < V_.size() ; i++)push_back_sub(CharType(V_[i]));
    }

    // 文字 c を Suffix Tree 構成元の列の末尾に追加する
    void push_back(CharType c){
        push_back_sub(c);
    }

    // 構築を終了する。
    void determine(){
        determine_sub();
    }

    // 列 V_ をもとに Suffix Tree を構築し、構築を終了する。
    template<class ArrayType, typename = std::enable_if_t<
        std::is_same_v<typename std::decay<ArrayType>::type, std::string>
        || std::is_same_v<typename std::decay<ArrayType>::type, std::vector<CharType>>>>
    void build(const ArrayType& S_){
        initialize();
        push_back(S_);
        determine();
    }

    // 列 X が、Suffix Tree の構成元の列 S の部分文字列として含まれるかどうか
    template<class ArrayType, typename = std::enable_if_t<
        std::is_same_v<typename std::decay<ArrayType>::type, std::string>
        || std::is_same_v<typename std::decay<ArrayType>::type, std::vector<CharType>>>>
    bool find(const ArrayType& X) const {
        TreePlace p_data = from_root(true);
        for(CharType x : X)if(!walkdown(p_data,x))return false;
        return true;
    }


    // 列 X が、Suffix Tree の構成元の列 S の部分文字列としていくつ含まれるか
    template<class ArrayType, typename = std::enable_if_t<
        std::is_same_v<typename std::decay<ArrayType>::type, std::string>
        || std::is_same_v<typename std::decay<ArrayType>::type, std::vector<CharType>>>>
    int count(const ArrayType& X){
        init_tree_dp();
        TreePlace p_data = from_root(true);
        for(CharType x : X)if(!walkdown(p_data,x))return 0;
        if(p_data.to == nullptr)return p_data.from->SubTreeEndPointCount;
        return p_data.to->SubTreeEndPointCount;
    }

    // Suffix Tree の根から列 X に沿って移動できるだけ移動し、最終的な位置データを返す
    template<class ArrayType, typename = std::enable_if_t<
        std::is_same_v<typename std::decay<ArrayType>::type, std::string>
        || std::is_same_v<typename std::decay<ArrayType>::type, std::vector<CharType>>>>
    TreePlace search(const ArrayType& X) const {
        TreePlace p_data = from_root(true);
        for(CharType x : X)if(!walkdown(p_data,x))break;
        return p_data;
    }


    /*
        Suffix Tree の元の文字列 S の [l,r) の部分文字列に対応する位置データを返す。
            |- 部分文字列に対応する定数サイズオブジェクトを O(logN) 時間で取得できておいしい。
            |- 返り値の位置データは、自身が表す文字列に関して非明示的 (refering が false)
            |- 長さ 0 の区間 [x,x) は空文字列に対応
    */
    TreePlace search(int l , int r ){
        // S の l 文字目から始まる接尾辞方向に (r-l) 文字進んだ位置が答え
        init_LCA();
        init_basic_data();
        assert(0 <= l && l <= r && r <= S.size());// 空文字列も OK
        TreePlace p_data = from_root(false);// (root,nullptr,0,{}) のデータ
        Node* border = m_SuffixNodes[l];
        
        // ギリギリ (r-l) 文字以上まで親方向に戻る (ダブリング)
        for(int k = int(m_parent.size())-1; k >= 0 ; k--){
            int y = m_parent[k][border->id];
            if(y < 0)continue;
            if(m_Nodes[y]->path_label_length() >= r-l)border = m_Nodes[y];
        }
        // さらに 1 つ戻ることで、S[l,r) に到達できる直近のノードになる
        if(border->path_label_length() > (r-l)){
            border = border->parent;
            p_data.to = border->child[S[l + border->path_label_length()]];
            p_data.place = (r-l) - border->path_label_length();
        }
        p_data.from = border;
        return p_data;
    }


    // 頂点 x の位置に対応する文字列を含む接尾辞の開始位置を返す (0-index)
    // 複数存在する場合はいずれか 1 つを返す
    int decode_suffix(const Node* x) const {
        assert(x != nullptr);
        if(x == root)return m_EndS;
        return x->cleft() - x->parent->path_label_length();
    }   

    // 位置 p_data に対応する文字列を含む接尾辞の開始位置を返す (0-index)
    // 複数存在する場合はいずれか 1 つを返す
    int decode_suffix(const TreePlace& p_data) const {
        if(p_data.to!=nullptr)return decode_suffix(p_data.to);
        return decode_suffix(p_data.from);
    }


    // 頂点番号で普通にLCA
    int LCA(int u , int v){
        init_LCA();
        init_basic_data();
        assert(0 <= u && u < tree_size());
        assert(0 <= v && v < tree_size());
        if(m_tree_depth[u] > m_tree_depth[v])std::swap(u,v);
        for(int k = int(m_parent.size())-1 ; k >= 0 ; k--){
            if(m_parent[k][v] < 0)continue;
            if(m_tree_depth[m_parent[k][v]] < m_tree_depth[u])continue;
            v = m_parent[k][v];
        }
        if(u == v)return u;
        for(int k = int(m_parent.size())-1 ; k >= 0 ; k--){
            if(m_parent[k][u] == m_parent[k][v])continue;
            u = m_parent[k][u];
            v = m_parent[k][v];
        }
        return m_parent[0][u];
    }


    // ポインタで示された頂点 u と v の LCA。返り値もポインタ。
    Node* LCA(const Node* u, const Node* v){
        assert(u != nullptr);
        assert(v != nullptr);
        // 同じ木の頂点かを確認
        assert(get_node(u->id) == u);
        assert(get_node(v->id) == v);
        return m_Nodes[LCA(u->id , v->id)];
    }

    // 位置データ sd と、頂点ポインタ v の LCA。
    // LCA が 辺の途中になる可能性もあるので TreePlace で返す。
    TreePlace LCA(const TreePlace& sd, const Node* v){
        // 位置 sd が根から v までのパス上にある場合
        if(sd.to!=nullptr && LCA(sd.to , v) == sd.to)return sd;    
        TreePlace res = from_root();
        res.from = LCA(sd.from , v);
        return res;
    }

    // 位置データ sd と、頂点ポインタ v の LCA。
    // LCA が 辺の途中になる可能性もあるので TreePlace で返す。
    TreePlace LCA(const Node* v , const TreePlace& sd){return LCA(sd,v);}

    // 位置データ sd と、位置データ sp の LCA。
    // LCA が 辺の途中になる可能性もあるので TreePlace で返す。
    TreePlace LCA(TreePlace sd ,TreePlace sp){
        TreePlace res = from_root();
        if(sd.to != nullptr && sp.to != nullptr){
            if(sd.to == sp.to && sd.from == sp.from){// 同じ辺の上の位置の場合
                if(sd.path_label_length() < sp.path_label_length())return sd;// 浅い方が答え
                return sp;
            }
            // 同じ辺にいない場合、深い方は from まで下げて良い
            if(sd.path_label_length() > sp.path_label_length()){
                // sp が深い方で標準化
                TreePlace temp = sd;
                sd = sp;
                sp = temp;
            }
            return LCA(sd,sp.from);
        }else if(sd.to == nullptr && sp.to != nullptr)return LCA(sp , sd.from);
        else if(sp.to == nullptr && sd.to != nullptr)return LCA(sd , sp.from);
        res.from = LCA(sd.from , sp.from);
        return res;
        
    }

    // root を表す位置データを返す。
    // refering が false なら対応する列が非明示的な位置データを作って返す。
    TreePlace from_root(bool refering = true) const {
        TreePlace res;
        res.from = root;
        res.to = nullptr;
        res.place = 0;
        res.refering = refering;
        res.refer = {};
        return res;
    }

    // 頂点番号が v であるノードを返す
    const Node* get_node(int v){
        init_basic_data();
        assert(0 <= v && v < tree_size());
        return m_Nodes[v];
    }

    // i 番目の suffix の終端に対応するノードを返す(0-index)
    const Node* get_suffix_node(int i){
        init_basic_data();
        assert(0 <= i && i < m_SuffixNodes.size());
        return m_SuffixNodes[i];
    }

    // 根ノードを返す
    const Node* get_root() const {
        return root;
    }

    // 頂点 v の深さ(根から辺を何本経由するか)
    int get_depth_as_tree(int v){
        init_basic_data();
        assert(0 <= v && v < tree_size());
        return m_tree_depth[v];
    }

    // 構築元の列 S の S[i] を返す (0-index で read-only)
    const CharType operator [](int i) const {
        assert(0 <= i && i < S.size());
        return S[i];
    }

    // 構築元の列 S の S.size()
    const int size() const {
        int res = S.size();
        return res;
    }

    // 構築元の列 S の連続部分列の種類数。空文字列も含む。
    long long KindOfSubSequence(){
        init_tree_dp();
        // 空文字列は DP でカウントされないのでここで加算
        return root->SubTreeKindOfPrefix + 1;
    }

    // Suffix Tree の頂点数
    const int tree_size() const {
        return m_vertex_id;
    }

    // 木の辺を頂点番号のペアとして、辺集合を vector で返す
    std::vector<std::pair<int,int>> edges() const {
        std::vector<Node*> dfs_ = dfs(false);
        std::vector<std::pair<int,int>> res;
        for(Node* x :dfs_){
            if(x->parent==nullptr)continue;
            res.push_back({x->parent->id , x->id});
        }
        return res;
    }

    // dfs 順の Node* を返す (辞書順に dfs するかを引数で指定)
    std::vector<Node*> dfs(bool lexicographical = false) const {
        std::vector<Node*> res;
        if(root == nullptr)return res;
        std::stack<Node*> s;
        s.push(root);
        while(!s.empty()){
            Node* now = s.top();s.pop();
            res.push_back(now);
            std::vector<Edge> nxt;
            for(Edge nx : now->child)if(nx.second != nullptr)nxt.push_back(nx);
            // 辺の最初の文字でソート
            if(lexicographical)sort(nxt.rbegin() , nxt.rend() , compare_edge);
            for(Edge nx : nxt)s.push(nx.second);
        }
        return res;
    }


    /* ==============================================================================================
    ** 内蔵している典型問題たち =========================================================================
    ** ============================================================================================*/
    
    /*
        このクラスが保持する列の Suffix Array を返す (0-index)
            |- 空列は辞書順最小の接尾辞とし、その suffix_id を |S| とする
    */
    std::vector<int> suffix_array(){
        if(!m_determined)calc_error();
        std::vector<Node*> s = dfs(true);
        std::vector<int> res;
        for(Node* now : s){
            if(!now->isEndPoint())continue;
            res.push_back(now->suffix_id);
        }
        return res;
    }
    

    /*
        列 S の 辞書順で K 番目の連続部分列
            |- 返り値は vector<CharType>
            |- K = 0 なら空の列を返す
    */
    std::vector<CharType> get_Kth_subSeq(long long K){
        init_tree_dp();
        assert(root != nullptr);
        assert(0 <= K && K < KindOfSubSequence());
        
        Node* now = root;
        long long acum = 0;
        std::vector<CharType> res(0);
        if(K == 0)return res;

        while(K > acum){
            std::vector<Edge>cand;
            for(Edge pr : now->child)if(pr.second!=nullptr)cand.push_back(pr);
            // 辺の最初の文字の昇順でソート
            sort(cand.begin(),cand.end(),compare_edge);

            for(Edge pr : cand){
                Node* nx = pr.second;
                int nx_edge_num = nx->label_length();// 次の頂点への辺上のprefixの種類数
                // 辺の先に答えがある場合
                if(nx->SubTreeKindOfPrefix + nx_edge_num + acum >= K){
                    if(acum + nx_edge_num >= K){// 答えとなる文字列が、次の頂点への辺の上にあるとき
                        for(int i = nx->cleft() ; i < nx->cleft() + K - acum ; i++ )res.push_back(S[i]);
                        return res;
                    }else{
                        for(int i = nx->cleft() ; i < nx->cright() ; i++ )res.push_back(S[i]);
                        now = nx;// 辺の先に移動 
                        acum += nx_edge_num;
                        break;// さっきいたノードからは抜ける
                    }
                }else{
                    acum += nx->SubTreeKindOfPrefix + nx_edge_num;// この辺の先に答えはないので、次の辺を見る
                } 
            }
        }
        assert(0);
        return res;
    }


    /*   
        Copyright © (c) okoteiyu 2024. 
        Released under the MIT license(https://opensource.org/licenses/mit-license.php) 
    */
};
