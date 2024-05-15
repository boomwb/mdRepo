# Git学习

1.下载然后设置邮箱和用户名

```
git config --global user.name "Your Name"
git config --global user.email "email@example.com"
```

  查询

```
git config user.email
 git config user.name 
git config --list
```

2.创建版本库(create repository)

  创建一个空目录

  通过`git init`命令把这个目录变成Git可以管理的仓库

  所有的版本控制系统，其实只能跟踪文本文件的改动，比如TXT文件，网页，所有的程序代码等等，Git也不例外。版本控制系统可以告诉你每次的改动，比如在第5行加了一个单词“Linux”，在第8行删了一个单词“Windows”。而图片、视频这些二进制文件，虽然也能由版本控制系统管理，但没法跟踪文件的变化，只能把二进制文件每次改动串起来，也就是只知道图片从100KB改成了120KB，但到底改了啥，版本控制系统不知道，也没法知道。

  Microsoft的Word格式是二进制格式，因此，版本控制系统是没法跟踪Word文件的改动的.

一个文件放到Git仓库只需要两步:

  1.`git add `把文件添加到仓库

  2.`git commit` 把文件提交到仓库

​    git commit -m "本次提交的说明"

```
git init 临时租用了一个场地作为演出区

git status 寻问各文件的状态，如新来的（add）、离开的（delete）、补妆的（modify）等。

git add 将补妆好的、新来的文件登记到演出准备（Staging Environment）名册中，注销演技老套、生病而离开的文件

git commit 便是把文件推到演出区进行录播，供万人在线随时欣赏；当文件临时补妆返场，也可通过 -a ，在寻问过文件 status 的情况下，跳过登记阶段，再次录播，并剪辑、更新原有录像。

git log 提交记录
git log --oneline 简介的提交记录



git reset ID号 或者  HEAD^ 回退版本,默认mixed
					工作区    暂存区
git reset --soft    保留	    保留
git reset --hard    丢弃		保留
git reset --mixed	保留		丢弃

git relog 查看操作记录,可用于误操作

git diff 比较工作区和暂存区的差异
git diff HEAD 比较工作区和版本库的差异
git diff --cache 比较暂存区和版本库的差异
git diff ID ID 比较两个版本的差异
HEAD指向分支的最新提交节点
HEAD~ HEAD^ 上一个版本

git ls-files 查看暂存区的文件

git rm 从暂存区注销该文件，并删除工作区文件（delete）
git rm --cached 只从从暂存区中注销该文件

.gitgnore文件



git restore --staged  是若干次add、rm --cached的逆向回退操作，撤销之前的登记操作**，**直到最近一次的commit 提交。

git restore 撤销工作区未登记的文件的删除、修改和创建（撤销会使新创建的文件处于Untracked的状态，但不会删除该文件），直到最近一次的登记。
git restore  撤销工作区已登记的文件的删除、修改，直到最近一次的登记。

git restore --staged 
是若干次add、rm --cached的逆向回退操作，撤销之前的登记操作，直到最近一次的commit 提交，被注销的文件处于Untracked的状态，但文件不会被删除
```



```
echo "# gitTest" >> README.md # 在仓库里输出一个名字为README.md的文件，有一行文字为# gitTest，因为md是markdown语法，所以看到的是h1标题的文字
git init
git add . # 默认提交所有修改入管理器
git commit -m "my first commit " # 提交入暂存区 -m 引入标注，引号内为标注内容
git remote add Wythe https://github.com/WytheO/gitTest.git #Wythe 是后面那串网址的别名，提交就输别名就可以了，不然每次都要把网址输一遍很麻烦
git push -u Wythe master #Wythe 就代表那个网址，master代表分支为主分支
# 如果你是clone的别人的代码，上传时请创建分支（branch）


```

3.三个区

工作区-git init-> 暂存区->git commit 本地仓库

4.git文件的状态

  未跟踪(Untrack)  未修改(Unmodified)已修改(Modedified)已暂存(Staged)

![image-20240501235931025](E:\Git_Projecr\Typora_project\Typora_pic\image-20240501235931025.png)



4.远程仓库

ssh协议,必须先配置秘钥

https,不推荐

ssh-keygen -t rsa -b 4096

```
git push 推送到远程仓库
git pull 拉取
```



SVN与小乌龟tortoisegit

[手把手教你 GitLab 的安装及使用 - 简书 (jianshu.com)](https://www.jianshu.com/p/b04356e014fa)

[【TortoiseGit】TortoiseGit安装和配置详细说明-CSDN博客](https://blog.csdn.net/weixin_44299027/article/details/121178817)

ROM与RAM

[关于RAM、SRAM、DRAM、ROM、RRPROM、FLASH的理解 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/70273183)

[RAM和ROM的区别_百度知道 (baidu.com)](https://zhidao.baidu.com/question/81373514.html)



# 问题集合

常规的MCU外设如STM32一样，通常是一个UART外设的所有中断请求都是共用同一个中断号，然后在该中断服务程序里面判断外设的标志位，得到中断源，并做出在相应和处理。

但是HC32F460就完全不一样了，UART外设里的每个中断标志位，都是独立请求中断，必须单独给分配中断号，这样一来，一个UART就可以有5个中断服务程序了，编程其实更麻烦了。
————————————————

















