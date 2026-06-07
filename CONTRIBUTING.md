# Contributing

本文件说明本仓库的资源维护规则，供作者、助教和课程组协作时参考。

## Repository Layout

章节资源统一放在 `resources/chapter-XX/` 下：

```text
resources/chapter-XX/
├── slides/
├── code/
├── labs/
├── assignments/
└── answers/
```

跨章节通用资源放入 `resources/common/`。跨章节项目工程放入 `projects/`。

## File Naming

- 目录和文件名优先使用英文、数字和连字符。
- 章节目录使用 `chapter-01`、`chapter-02`、...、`chapter-24`。
- 避免在文件名中使用空格、括号、中文标点和特殊符号。
- 大文件不要直接提交到主仓库，优先通过 GitHub Releases 发布。

## Resource Updates

新增或修改资源时，请同步更新：

- `docs/update-log.md`：记录资源变更。
- `docs/guide.md`：如下载方式、目录结构或运行方式发生变化，需要同步更新。
- `docs/errata.md`：只有已经确认的勘误才写入公开勘误表。

## Answers

参考答案可以公开，统一放入对应章节的 `resources/chapter-XX/answers/`。答案文件应标明对应章节、题号和更新时间。

## Book PDF Releases

教材 PDF 和其他较大的教材资料通过 GitHub Releases 发布。发布新版 PDF 的推荐流程：

1. 将新版 PDF 临时放到 `materials/book/`，使用当前约定的英文文件名。
2. 提交并推送包含新版 PDF 的 commit。
3. 创建并推送新的 `book-draft-*` tag。
4. 等待 `.github/workflows/publish-book-release.yml` 创建 GitHub Release 并上传 PDF。
5. 在后续 commit 中从主仓库移除 PDF，只保留 Release 链接说明。

## Issues

读者反馈优先使用 GitHub Issues：

- 勘误反馈使用 `errata` label。
- 资源问题或资源请求使用 `resource` label。
- 未分类问题使用 `needs-triage` label。

如发现模板或标签不够用，可更新 `.github/labels.json` 并推送，标签同步工作流会尝试自动创建或更新标签。
