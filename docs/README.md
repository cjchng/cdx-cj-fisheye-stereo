# Documentation / 文件索引

Generated documents for the fisheye stereo app and its calibration/research background. Application guides are in English; the research notes and teaching handouts pair English with Traditional Chinese.

本資料夾收錄雙魚眼程式的操作文件及校正／研究背景。程式指南為英文；研究筆記與教學講義為英語及繁體中文對照。

## Application guides

| Document | Link |
|---|---|
| C++ library, CLI, calibration workflow and REST API | [App/API guide](Job-006_cdx-fisheye-stereo-guide.md) |
| Docker build, Compose, utilities and verification status | [Docker guide](Job-007_cdx-fisheye-docker-guide.md) |
| Machine-readable REST contract | [OpenAPI](../api/openapi.yaml) |

The [repository README](../README.md) and [Docker README](../docker/README.md) remain the entry points for running the current app. The copied guides record the generated implementation documentation. Docker image/runtime verification is still incomplete because the dependency download was interrupted; native tests passed.

## Research notes / 研究筆記

See the [bilingual reading index](Job-003_cdx-reading-index.md) for the reading order, evidence limitations and online source papers.

| Topic / 主題 | Markdown | PDF |
|---|---|---|
| KB and OCam models / KB 與 OCam 模型 | [Read](Job-003_cdx-01-kb-ocam-models.md) | [PDF](Job-003_cdx-01-kb-ocam-models.pdf) |
| Calibration workflows / 校正流程 | [Read](Job-003_cdx-02-calibration-workflows.md) | [PDF](Job-003_cdx-02-calibration-workflows.pdf) |
| Projection centre / 投影中心 | [Read](Job-003_cdx-03-projection-center.md) | [PDF](Job-003_cdx-03-projection-center.pdf) |
| 3D metrology evidence / 三維度量證據 | [Read](Job-003_cdx-04-3d-metrology-evidence.md) | [PDF](Job-003_cdx-04-3d-metrology-evidence.pdf) |
| Native-ray measurement / 原生光線量測 | [Read](Job-003_cdx-05-native-ray-measurement.md) | [PDF](Job-003_cdx-05-native-ray-measurement.pdf) |
| Trajectory accuracy / 軌跡精度 | [Read](Job-003_cdx-06-trajectory-accuracy.md) | [PDF](Job-003_cdx-06-trajectory-accuracy.pdf) |
| Beyond 180 degrees / 超過 180 度 | [Read](Job-003_cdx-07-beyond-180-evidence.md) | [PDF](Job-003_cdx-07-beyond-180-evidence.pdf) |

## Teaching handouts / 教學講義

| Topic / 主題 | Markdown | PDF |
|---|---|---|
| Angular sampling and solid angle / 角取樣與立體角 | [Read](Job-004_cdx-A-angular-sampling-teaching.md) | [PDF](Job-004_cdx-A-angular-sampling-teaching.pdf) |
| OCam versus KB / OCam 與 KB 比較 | [Read](Job-004_cdx-B-kb-ocam-teaching.md) | [PDF](Job-004_cdx-B-kb-ocam-teaching.pdf) |

The handouts include questions and answers, derivations, and further reading. Their shared analytical figure is in [Job-004_cdx-assets](https://github.com/cjchng/cdx-cj-fisheye-stereo/tree/main/docs/Job-004_cdx-assets).

## Scope and provenance / 範圍與來源

- Original `Job-NNN` filenames are retained to identify generated documents. Research/teaching documents date from 2026-09-29; app/container guides date from 2026-10-04.
- The nine PDFs and teaching figure are unchanged copies. Repository copies of the Markdown guides/index adjust navigation links to work here. The original workspace documents remain unchanged.
- Research notes discuss broader models, noncentrality and fields beyond 180 degrees. They do not imply those capabilities are implemented by this app, which uses OpenCV's fisheye model with forward-hemisphere observations.
- These are generated explanatory/research documents, not new experimental results or substitutes for their cited original papers. Third-party papers are linked online rather than copied into this repository.
- Update the copied app/container guides when implementation behaviour changes. Keep scientific claims and their stated experimental conditions together.

PDF files can be read or downloaded from GitHub. Relative PDF links between the two teaching handouts work when both files are downloaded into the same directory; browser PDF-viewer support varies.
