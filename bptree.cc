#include "bptree.h"
#include <vector>
#include <sys/time.h>

NODE *alloc_leaf(NODE *parent);
NODE *alloc_parent(NODE *parent);
NODE* nonleaf_split(NODE* current_internal, int key, NODE* old_chi, NODE* new_chi);

struct timeval
cur_time(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t;
}

void
print_tree_core(NODE *n)
{
	printf("["); 
	for (int i = 0; i < n->nkey; i++) {
		if (!n->isLeaf) print_tree_core(n->chi[i]); 
		printf("%d", n->key[i]); 
		if (i != n->nkey-1 && n->isLeaf) putchar(' ');
	}
	if (!n->isLeaf) print_tree_core(n->chi[n->nkey]);
	printf("]");
}

void
print_tree(NODE *node)
{
	print_tree_core(node);
	printf("\n"); fflush(stdout);
}

NODE *
find_leaf(NODE *node, int key)
{
	int kid;

	if (node->isLeaf) return node;
	for (kid = 0; kid < node->nkey; kid++) {
		if (key < node->key[kid]) break;
	}

	return find_leaf(node->chi[kid], key);
}

NODE *
insert_in_leaf(NODE *leaf, int key, DATA *data)
{
	int i;
	if (key < leaf->key[0]) {
		for (i = leaf->nkey; i > 0; i--) {
			leaf->chi[i] = leaf->chi[i-1] ;
			leaf->key[i] = leaf->key[i-1] ;
		} 
		leaf->key[0] = key;
		leaf->chi[0] = (NODE *)data;
	}
	else {
		for (i = 0; i < leaf->nkey; i++) {
			if (key < leaf->key[i]) break;
		}
		for (int j = leaf->nkey; j > i; j--) {		
			leaf->chi[j] = leaf->chi[j-1] ;
			leaf->key[j] = leaf->key[j-1] ;
		} 
    /* CodeQuiz */
		leaf->key[i] = key;
		leaf->chi[i] = (NODE *)data;
	}
	leaf->nkey++;

	return leaf;
}

NODE *
insert_into_parent(NODE *current_parent, int key, NODE *old_chi, NODE *new_chi){

// 1: if N is the root of the tree then
// 2: Create a new node R containing N,K′,N′
// 3: Make R the root of the tree return
// 4: end if
// 5: Let P = parent(N)
// 6: if P has less than n pointers then
// 7: insert(K′, N′) in P just after N
	if (current_parent->nkey < N-1){
		int i;
		if(current_parent->nkey == 0){
			current_parent->key[0] = key;
			current_parent->chi[0] = (NODE *)old_chi;
			current_parent->chi[1] = (NODE *)new_chi;
		}
		else {
			//キーを挿入する場所を探して、そこに挿入する
			for (i=0;current_parent->key[i] < key && i < current_parent->nkey; i++); //挿入場所はcurrent_parent->key[i]
			// 後ろのキーをずらす
			for (int j=0; j<current_parent->nkey-i; j++){
				current_parent->key[current_parent->nkey-j] = current_parent->key[current_parent->nkey-j-1];
				current_parent->chi[current_parent->nkey-j+1] = current_parent->chi[current_parent->nkey-j];
			}
			current_parent->key[i] = key;
			current_parent->chi[i+1] = (NODE *)new_chi;
			// current_parent->chi[i] = (NODE *)old_chi;
		}
	current_parent->nkey++;

	} else{
		nonleaf_split(current_parent, key, old_chi, new_chi);
	}
	return current_parent;
}

NODE* 
nonleaf_split(NODE* current_internal, int key, NODE* old_chi, NODE* new_chi){
	//split
	// 9: Copy P to a block of memory T that hold P and (K′&N′)
	// 10: Insert(K′, N′) into T just after N
	// 11: Erase all entries from P
	// 12: Create node P′
	// 13: CopyT.P1,...,T.P⌈(n+1)/2⌉ intoP
	// 14: Let K′′ = T.K⌈(n+1)/2⌉
	// 15: Copy T.P⌈(n+1)/2⌉+1,...,T.Pn+1 into P′
	// 16: insert in parent(P,K′′,P′)
	// 17: end if

	// すでに親ノードはいっぱい
	if (current_internal->parent == NULL){
		current_internal->parent = alloc_parent(NULL);
		Root = current_internal->parent;
	}

	TEMP *temp;
	temp = (TEMP *)calloc(1, sizeof(TEMP)); //TEMPを一個割り当てる
	temp->isLeaf = false;
	temp->nkey = N;

	//分割する前提でtempにソートされたキーとポインタをコピーしてくる
	if (key < current_internal->key[0]) {
		for (int i = current_internal->nkey+1; i > 1; i--) {
			temp->chi[i] = current_internal->chi[i-1];
			temp->key[i] = current_internal->key[i-1];
		} 
		temp->key[0] = key;
		temp->key[1] = current_internal->key[0];
		temp->chi[0] = (NODE *)old_chi; //old_chiでないことに注意、、やっぱoldでした
		temp->chi[1] = (NODE *)new_chi;
	}
	else {
		for (int i=0; key > current_internal->key[i]; i++){ //前半をまずコピー
			temp->key[i] = current_internal->key[i];
			temp->chi[i] = current_internal->chi[i];
		}

		temp->key[(temp->nkey)-1] = key;
		temp->chi[(temp->nkey)-1] = (NODE *)old_chi;
		temp->chi[(temp->nkey)] = (NODE *)new_chi;
	
		for (int i=temp->nkey+1; i<N+1; i++){ //後半をコピー
			temp->key[i] = current_internal->key[i-1];
			temp->chi[i] = current_internal->chi[i-1];
		}
	}

	//以上でコピー完了
	//次はtempを分割して分配する
	NODE *new_internal;
	new_internal = alloc_leaf(current_internal->parent); //parentは上でallocしてるから渡せる
	new_internal->parent = current_internal->parent; 
	new_internal->isLeaf = false;

	int mid; //splitする位置を決める. これは左にmid個残すという意味。leaf_splitと違い右にはmid-1個しか残さない
	if (N%2 == 0) mid = N/2;
	else mid = (N+1)/2;

	current_internal->nkey = 0;
	int i = 0;
	for (; i<mid; i++){ //分割した前半をleafにコピー
		current_internal->key[i] = temp->key[i];
		current_internal->chi[i] = temp->chi[i];
		current_internal->nkey++;
	}

	int j = i;  // i の最終値に基づいて j を計算
	if (j < N) {    // このチェックを追加して、範囲外アクセスを防ぐ
		current_internal->chi[j] = temp->chi[j];
	}

	for (int i = 0; i < N - mid - 1; i++) {
		if (i + mid + 1 >= N) break; // 配列範囲外を避ける
		new_internal->key[i] = temp->key[i + mid + 1];
		new_internal->chi[i] = temp->chi[i + mid + 1];
		new_internal->chi[i]->parent = new_internal; // ここを追加
		new_internal->nkey++;
	}
	new_internal->chi[N - mid - 1] = temp->chi[N]; // ここも配列範囲外を避ける
	new_internal->chi[N - mid - 1]->parent = new_internal; // ここを追加

	//親ノードにを繰り上げる
	insert_into_parent(current_internal->parent, temp->key[mid],current_internal, new_internal);
	// new_leaf->parent->chi[leaf->parent->nkey] = new_leaf;
	free(temp);
	return new_internal;

	return NULL;
}

NODE *
leaf_split(NODE *leaf, int key, DATA *data)
{
	if (leaf->parent == NULL){
		leaf->parent = alloc_parent(NULL);
		Root = leaf->parent;
	}

	TEMP *temp;
	temp = (TEMP *)calloc(1, sizeof(TEMP)); //TEMPを一個割り当てる
	temp->isLeaf = true;

	
	//leafからtempにkeyとdataをコピーする
	int i;
	if (key < leaf->key[0]) {
		for (i = leaf->nkey; i > 0; i--) {
			temp->chi[i] = leaf->chi[i-1] ;
			temp->key[i] = leaf->key[i-1] ;
			temp->nkey++;
		} 
		temp->key[0] = key;
		temp->chi[0] = (NODE *)data;
		temp->nkey++;
	}
	else {
		for (int i=0; key > temp->nkey&&temp->nkey<N; i++){ //前半をまずコピー
			temp->key[i] = leaf->key[i];
			temp->chi[i] = leaf->chi[i];
			temp->nkey++;
		}

		temp->key[(temp->nkey)-1] = key;
		temp->chi[(temp->nkey)-1] = (NODE *)data;
	
		for (int i=temp->nkey+1; i<N+1; i++){ //後半をコピー
			temp->key[i] = leaf->key[i-1];
			temp->chi[i] = leaf->chi[i-1];
			temp->nkey++;
		}
	}
	
	//以上でコピー完了
	//次はtempを分割して分配する
	NODE *new_leaf;
	new_leaf = alloc_leaf(leaf->parent); //新しいleafを割り当て、初期化する
	new_leaf->parent = leaf->parent; //まだalloc_parentしてないから意味がない→fixed.


	int mid; //splitする位置を決める. これは左にmid個残すという意味
	if (N%2 == 0) mid = N/2;
	else mid = (N+1)/2;


	// leaf->nkey = mid; //削除の代わりにnkeyを変更する,,だけじゃダメ！！！それだと一番右以外のインサートができなくなる -> fixed
	leaf->nkey = 0;
	for (int i=0; i<mid; i++){ //分割した前半をleafにコピー
		leaf->key[i] = temp->key[i];
		leaf->chi[i] = temp->chi[i];
		leaf->nkey++;
	}

	for (int i=0; i<N-mid; i++){ //分割した後半をnew_leafにコピー
		new_leaf->key[i] = temp->key[i+mid];
		new_leaf->chi[i] = temp->chi[i+mid];
		new_leaf->nkey++;
	}

	//親ノードにを繰り上げる
	insert_into_parent(leaf->parent, new_leaf->key[0],leaf, new_leaf);
	free(temp);
	return new_leaf;
}


NODE *
alloc_leaf(NODE *parent)
{
	NODE *node;
	if (!(node = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	node->isLeaf = true;
	node->parent = parent;
	node->nkey = 0;

	return node;
}

NODE *
alloc_parent(NODE *parent){
	NODE *new_parent;
	if (!(new_parent = (NODE *)calloc(1, sizeof(NODE)))) ERR;
	new_parent->isLeaf = false;
	new_parent->parent = NULL;
	new_parent->nkey = 0;

	return new_parent;
}

void 
insert(int key, DATA *data)
{
	NODE *leaf;

	if (Root == NULL) {
		leaf = alloc_leaf(NULL);
		Root = leaf;
	}
  else {
    leaf = find_leaf(Root, key);
  }
	if (leaf->nkey < (N-1)) {
		insert_in_leaf(leaf, key, data);
	}
	else { // split
		leaf_split(leaf, key, data);
	}
}

void
init_root(void)
{
	Root = NULL;
}

int 
interactive()
{
  int key;

  std::cout << "Key: ";
  std::cin >> key;

  return key;
}
int main(int argc, char *argv[])
{
    struct timeval begin, end;

    init_root();

    printf("-----Insert-----\n");
    begin = cur_time();

	for (int i = 100; i >1; i--) {
		insert(i, NULL);
	}
	print_tree(Root);
    end = cur_time();

    return 0;
}
// int
// main(int argc, char *argv[])
// {
//   struct timeval begin, end;

// 	init_root();

// 	printf("-----Insert-----\n");
// 	begin = cur_time();
//   while (true) {
// 	insert(interactive(), NULL);
//     print_tree(Root);
//   }
// 	end = cur_time();

// 	return 0;
// }