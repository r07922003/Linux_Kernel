![Observer Pattern](https://imgur-backup.hackmd.io/tfZATIn.png)

## Context：
你在購物網站上看到一個很熱門且價格超便宜的產品，正當你想要下單購買的時候，你發現這個產品目前「售完補貨中」。

## Problem： 
如何得知商品已到貨？

## Force： 
你可以定期連回這個網站查詢產品是否到貨，但是這樣做真的很浪費時間。 為了知道商品是否到貨而搬到購物網站的倉庫去住（緊密耦合），是一件很愚蠢的行為。除了你以外，還有很多人都對這個商品有興趣
如果商品到貨而你沒有適時地收到通知，你會很火大。

## Solution：
請購物網站 (Subject) 提供「貨到通知」功能，讓你 (Observer) 可以針對有興趣的產品主動加入 (attach) 到「貨到請通知我」的名單之中。你必須將你的電子郵件地址 (update) 提供給購物網站，以便貨品到貨的時候購物網站可以立即通知 (notify) 你。當你對產品不再有興趣的時候，購物網站也要讓你可以從「貨到請通知我」的名單中移除 (detach)。

## reference
- [QMonkey/OOC-Design-Pattern](https://github.com/QMonkey/OOC-Design-Pattern/tree/master)