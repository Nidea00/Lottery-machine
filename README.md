# Lottery-machine
Boring Lottery machine for Interview of Probability Engineer.

Design a drawing machine with 2 output (Head prize and Second prize), following these requests below:

```
1. It draws a head prize with probability 5%, and draws a second prize with 10%.
2. Head prize and second prize will never be drawn at the same time.
3. Receive at least one prize in the first 5 draws.
4. Receive at least one prize in the consecutive 20 draws.
5. Design prize money with expected value 45.
```

我們依照要求A 設定的機率設計滿足要求F 的中獎金額，它應當滿足下述：

𝐸 = 0. 05 * 𝐵1 + 0. 10 *𝐵2

這裡 𝐸 為期望值，𝐵1 , 𝐵2 分別代表頭獎與貳獎的金額。代入期望值 45 可得之兩

變數的關係應滿足 𝐵1 + 2*𝐵2 = 900 ，並且理所當然地，𝐵1 >𝐵2 > 0 。

## 方案

1. 對於每一玩家，設置一初始變數 i = 15 ，用作紀錄未中獎次數
2. 抽獎發生時，執行一次 Bernoulli trial，機率 _p_ 定為： 𝑝 = 𝑐 + ( 1 − 𝑐) * ( 1 𝑖 9 )α

```
// 此處 c 為基礎機率、α 用於控制函數的增長速度，我們可透過多次抽樣
的方式尋照合適的 α 使得整體中獎率滿足要求。
a. 若成功，則以機率 𝑝=^13 再執行一次 Bernoulli trial
i. 若成功，輸出頭獎，i 歸零
ii. 若失敗，輸出貳獎，i 歸零
b. 若失敗，指派 i+1 予 i (i = i+1)
```

## 解釋

步驟 2. 的函數 𝑝 是一遞增函數：

𝑖= 0 時，𝑝=𝑐；

𝑖>= 19 時，𝑝= 1 。

這樣的設計能夠確保連續槓龜 19 次後，中獎機率為 1 ，我們能藉由調整 α 使得整
體中獎率趨向我們所要的機率。

我們姑且以 𝐴, 𝐵 分別表示樣本空間中抽中頭獎與貳獎的子集，根據要求
，𝐴 與 𝐵 沒有交集，意謂著：
𝑃(𝐴∩𝐵)= 0 且 𝑃(𝐴∪𝐵)=𝑃(𝐴)+𝑃(𝐵)
也就是說整體中獎機率 𝑃(𝐴∪𝐵)= 0. 15 。
而”已知中獎時，抽到頭獎的機率”為：

𝑃(𝐴|𝐴 ∪ 𝐵) = P(A)/(P(A)+P(B))

代入可得知，步驟 2.a. 設定的條件機率應為 ⅓ ，類似地 ”已知中獎時，抽到貳獎
的機率”則為 ⅔ 。

模擬結果：
我們能使用 grid search 或 binary search 等方法，去定位合適的 alpha
使用 mt19937 算法提供偽隨機數，seed 定為 42 ，當 alpha = 1.2375，經過
1,000,000 次抽獎後：
Ratio of Head Prize = 5.01%,
Ratio of Second Prize = 9.98%

請參考附件：drawing_machine.h, simulation.cpp


