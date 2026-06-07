---
layout: page
title: Release Process
---

# Release Process

本文件记录教材 PDF 和大文件资源的发布流程。

## Book PDF

教材 PDF 通过 GitHub Releases 发布，不长期保存在主仓库中。

当前 Release：

https://github.com/opentpu/FPGA-Digital-Circuits-and-System-Design/releases/tag/book-draft-2026-06-08

## Publishing a New Book PDF

1. 将新版 PDF 放到 `materials/book/`。
2. 保持 PDF 文件名为英文、数字和连字符。
3. 提交并推送包含新版 PDF 的 commit。
4. 创建并推送新的 tag，例如：

```bash
git tag -a book-draft-2026-07-01 -m "Book draft PDF 2026-07-01"
git push origin book-draft-2026-07-01
```

5. GitHub Actions 会运行 `.github/workflows/publish-book-release.yml`，创建 Release 并上传 PDF。
6. Release 创建成功后，在后续 commit 中从主仓库移除 PDF，并更新 `materials/book/README.md` 和 `docs/guide.md` 中的 Release 链接。
