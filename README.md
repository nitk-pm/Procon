# Procon 28 競技部門
香川高等専門学校高松キャンパスの競技部門チームのリポジトリです

# 開発の進め方
## セッティング
1. このリポジトリ(nitk-pm/Procon)をフォーク
2. フォークしたブランチをclone (git clone https://github.com/your-name/Procon)
3. このリポジトリをupstreamとしてリモートリポジトリに登録

## 機能の追加
1. 機能実装用ブランチをcloneしたリポジトリに作成 (git checkout -b my_feature_branch)
2. 機能実装用ブランチに変更点をcommit (git commit -m "Add some feature")
3. 自分のフォークしたリポジトリ(origin)にpush (upstreamにはpushしない)
4. GitHub上でこのリポジトリのdevelopブランチに対するPull Requestを作成する
### 間違ってupstreamにpushした場合
 * git revertで打ち消し、upstreamにpushする
### upstreamが変更された場合
1. git fetch upstreamでupstreamの変更点を持ってくる
2. git merge upstream developでupstreamの変更点を反映

### Pull Requestを出す際の注意
 * 自動でmerge出来ない(ボタンが灰色)の場合、上記の「upstreamが変更された場合」の手順でupstreamをローカルのdevelopに取り込み、
ローカルのdevelopを機能追加ブランチにmergeしてから再度pushする。

## Pull Requestへの対応
 * Pull Requestがあった場合、出した人以外の人がソースコードを見てコメントを残す。何か修正すべき点があれば具体的に書く
 * コメントが無い場合はSlack等で通知し、コメントを催促する。
