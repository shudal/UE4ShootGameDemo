# 以下是 骨骼动画 课后的
##### 演示

```
链接：https://pan.baidu.com/s/1XzodM2e-mdIL1y4uhyX3sQ 
提取码：ramg 
复制这段内容后打开百度网盘手机App，操作更方便哦
```

##### 作业的实现
* 实现了idle、跑、跳跃的基础动作表现。
	* 通过状态机实现这几个状态的切换。
	* 跑 是一个混合，包含从静止到跑起来的动画。

* 实现了角色的俯仰表现。通过 瞄准偏移 实现。俯仰表现 与 其他动作表现 进行了混合。
	* 俯仰时，因为 枪 通过AttachToComponent(Snap To)固定于手的骨骼上，因此枪也会随之旋转。
* 近战攻击有：左刺拳(J键)、右鞭腿(K键)、以及PowerMash(L键),还有个 被打的翻身(U键)。当移动时，近战攻击只有上半身的表现，这里通过分层动画实现(在腰那个骨骼那里分层)。跑起来的速度大于一定时，将有个bool变量b1表示人物已经跑起来，通过b1来判断是否应该使用分层动画。
* 按下Alt键时，有自由视角功能。通过 瞄准偏移 和 分层动画 实现的这个功能。

##### 作业之外的实现
* 俯仰表现 的 同步。在不做任何同步的情况下，client的 俯仰 会同步到server，server和其他client的俯仰 却不会同步到当前client。
    * 同步实现：角色弄个成员变量记录俯仰的数据，server将这个成员变量replicated到所有客户端。当客户端收到这个成员变量被改变而同步时，若这个角色不是local controlled的，则将这个成员变量作为俯仰的数据。

* 近战攻击动作 的 同步。角色弄个成员变量V1记录当前触发的近战攻击动作(Skill0,Skill1,...,NoneSkill)，同时将V1设成(replicated)。当这个成员变量改变时，client通过【rpc server】将这个成员变量同步到server(也就是 函数SetV1(type NewV1)  为rpc server)，接着因为V1是replicated的属性，所以V1又会被自动同步到其他client
* 主界面create session、search session、join session的实现。逻辑是用c++实现的，以便以后代码复用。网上没找到好的教程，花了不少时间看官方案例-多人射击游戏，主要是那个ShooterGameSession类，最终终于成功弄好了这套功能。
 

# 以下是 UMG 课后的：

##### UMG相关说明
* 角色个人信息显示出来了，包含角色的id和角色的昵称
* 游戏开始时，会从本地加载昵称，如果没有则是"NoneName"
* 游戏结束后，显示出最终排行榜，排行榜中的前两名可以修改自己的昵称。这个排行榜是通过list view做的。
    * 视频演示中，有3个玩家，第一轮游戏中最后一个保存到文件的玩家叫"second",因此在第二轮游戏开始后，编辑器界面中那个玩家的角色的昵称是"second"。

* 游戏除了通过鼠标左键射击外，还可以通过屏幕中的射击按钮射击。


###### 演示视频及 windows安装包

```
链接：https://pan.baidu.com/s/1LIMbnzg6tohpw6qTRxemMA 
提取码：b62n 
复制这段内容后打开百度网盘手机App，操作更方便哦
```



# 以下是 游戏模式 课后的：

##### 部分游戏逻辑说明
* 开始游戏后，玩家 生成 武器类 于 手上。
* 点击鼠标左键开火后，玩家类 调用自己拥有的 武器类 的 开火函数。
	* 开火函数 生成 弹丸，并给弹丸合适的方向。之后弹丸以 一定速度 飞出
		* 开火函数 是个rpc(server)调用自己拥有的
		* 弹丸 生成的位置是 枪口
	* 弹丸 碰撞靶子后，先获取 靶子的分数，接着再给 开火的玩家加分。
		* 靶子的分数 = 2.0 * 剩余时间秒数 / 10
* 玩家类 的 分数 记录在其PlayerState中，游戏的一轮时间记录在GameMode中。
* 项目大多数使用c++实现的，而不是 blueprint visual script
	* 蓝图类有：ui控件、弹丸、GameMode、靶子、武器。使用蓝图类是为了方便的修改类成员变量。
	
	
###### 演示视频及 windows安装包

```
链接：https://pan.baidu.com/s/1fcjc4y4fayNp1N9UT5dyhg 
提取码：9mtp 
复制这段内容后打开百度网盘手机App，操作更方便哦
```