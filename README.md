## 同期処理
「[並行プログラミング入門](https://www.oreilly.co.jp/books/9784873119595/)」に触発され、アプリケーション層に同期処理を提供している低レベルなinstruction(x86)を見ていく.

## x86における同期を提供する仕組み
自分のマシンで実行されてるプリミティブを見てみた
### lock プレフィクス
参考: [intel SDM](https://www.intel.co.jp/content/dam/www/public/ijkk/jp/ja/documents/developer/IA32_Arh_Dev_Man_Vol2A_i.pdf)
* このプレフィクスがinstructionの先頭についていると、その操作はアトミックに実行される

例)
```c
int num = 0
__sync_fetch_and_add(&num, 1);
```
上記のgccが提供してる組み込みの同期関数は、下のようなinstructionに展開される
```asm
     f0 83 45 f4 01          lock addl $0x1,-0xc(%rbp)
```
このadd命令は同じタイミングで複数コアが実行するようなことはない
### cmpxchgとcompare and swap
参考: [intel SDM](https://www.intel.co.jp/content/dam/www/public/ijkk/jp/ja/documents/developer/IA32_Arh_Dev_Man_Vol2A_i.pdf)
* そのまま引用
    * *（オペランドのサイズにしたがって）AL、AX、または EAX レジスタの値を第 1 オペ
ランド（デスティネーション・オペランド）と比較する。2 つの値が等しい場合は、
第 2 オペランド（ソース・オペランド）がデスティネーション・オペランドにロード
される。等しくない場合は、デスティネーション・オペランドが AL、AX、または
EAXレジスタにロードされる。*
* 雑にまとめると、比較、交換が一気に実行されるみたいな感じ.atomic処理の提供

例)
```c
int a = 0;
__sync_bool_compare_and_swap(&a, 42, 24);
```
下記のコードに展開される
```
b8 2a 00 00 00          mov    $0x2a,%eax
ba 18 00 00 00          mov    $0x18,%edx
f0 0f b1 15 91 2e 00    lock cmpxchg %edx,0x2e91(%rip)     
```


### xchgとtest and set
参考: [X86アセンブラ/データ転送命令](https://ja.wikibooks.org/wiki/X86%E3%82%A2%E3%82%BB%E3%83%B3%E3%83%96%E3%83%A9/%E3%83%87%E3%83%BC%E3%82%BF%E8%BB%A2%E9%80%81%E5%91%BD%E4%BB%A4)
```c
__sync_lock_test_and_set(&a, 3);
```
これが下記のようになる
```asm
xchg   %eax,-0xc(%rbp)
```
xchg命令は自動的にlockプレフィクスがついた時と同じ挙動になるらしい.


## まとめ
* 上位レイヤで提供されてるセマフォ、mutex, lockなどの排他制御の仕組みは、つまるところ下記に代表されるようなアルゴリズムなどを使って実現されてることが多い
  * cas
  * tas
* そして上記を実現するための「ハードウェアが提供する命令」としては、x86では
  * xchg
  * cmpxchg
  * lockプレフィクス  

  等がある

## todo
* mutex, lockを実際に実装してみる
