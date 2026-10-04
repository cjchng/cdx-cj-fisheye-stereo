# Seven-topic research pack / 七主題研究文件集

2026-09-29 · English + Traditional Chinese, paired by paragraph / 英文與繁體中文逐段對照

**EN.** This pack answers the seven questions about KB, Scaramuzza/OCamCalib and fisheye metrology. Each topic has an editable Markdown document and a matching PDF, with formulas, source links and explicit limits on the evidence. Read topics 01–03 for model fundamentals, 04–05 for measurement methods, and 06–07 for validation. Research coverage is targeted rather than a systematic review; no new camera experiment was performed.

**中。** 本文件集回答 KB、Scaramuzza／OCamCalib 及魚眼度量的七個問題。每個主題均有可編輯 Markdown 與相同內容的 PDF，包含公式、來源連結及證據適用限制。建議先讀 01–03 的模型基礎，再讀 04–05 的度量方法，最後讀 06–07 的驗證。這是針對性文獻研究，而非系統性回顧；本次沒有執行新的相機實驗。

## Downloads / 下載

**EN.** Browse the [repository document index](README.md) for all seven Markdown/PDF pairs and the related teaching and application guides. Third-party papers are linked to online sources below.

**中。** 請參閱[儲存庫文件索引](README.md)，取得七份 Markdown／PDF 及相關教材與程式指南；第三方論文以下方線上來源連結提供。

| Topic / 主題 | Editable / 可編輯 | Reading copy / 閱讀版 |
|---|---|---|
| 01 Mapping, inversion, FoV / 映射、求逆、視域 | [Markdown](Job-003_cdx-01-kb-ocam-models.md) | [PDF](Job-003_cdx-01-kb-ocam-models.pdf) |
| 02 Implementation workflows / 實施步驟比較 | [Markdown](Job-003_cdx-02-calibration-workflows.md) | [PDF](Job-003_cdx-02-calibration-workflows.pdf) |
| 03 Projection center and convergence / 投影中心與匯聚 | [Markdown](Job-003_cdx-03-projection-center.md) | [PDF](Job-003_cdx-03-projection-center.pdf) |
| 04 3D positions and dimensions / 三維位置與尺寸 | [Markdown](Job-003_cdx-04-3d-metrology-evidence.md) | [PDF](Job-003_cdx-04-3d-metrology-evidence.pdf) |
| 05 Direct native-ray measurement / 原生光線直接度量 | [Markdown](Job-003_cdx-05-native-ray-measurement.md) | [PDF](Job-003_cdx-05-native-ray-measurement.pdf) |
| 06 Trajectory accuracy / 軌跡精度 | [Markdown](Job-003_cdx-06-trajectory-accuracy.md) | [PDF](Job-003_cdx-06-trajectory-accuracy.pdf) |
| 07 Beyond 180° / 超過 180° 的證據 | [Markdown](Job-003_cdx-07-beyond-180-evidence.md) | [PDF](Job-003_cdx-07-beyond-180-evidence.pdf) |

## Findings to carry into the proposal / 可帶入計畫書的結論

**EN.** KB supplies pixel-to-ray mapping and is not intrinsically capped at 180°. Its usual radial model has one calibrated forward polynomial, while OCamCalib commonly derives a second approximation polynomial from its calibrated backward model. Runtime iteration, calibration optimization and physical-model validity are separate issues. Both standard models assume a common center; neither estimates the physical spread of ray origins merely by fitting a radial polynomial.

**中。** KB 提供像素到光線的映射，本身不以 180° 為上限。其常見徑向模型校正一個正向多項式；OCamCalib 通常由已校正的反向模型衍生第二個近似多項式。執行時迭代、校正最佳化及實體模型有效性是不同問題。兩種標準模型都假定共同中心，單憑徑向多項式擬合並不能估計實體光線原點的分散程度。

**EN.** Direct fisheye measurement without a pinhole raster is supported by geometry and implemented methods. Published dimensional examples include Scaramuzza's catadioptric checker reconstruction and Fu et al.'s fisheye wand measurements. However, reprojection pixels, trajectory metres, surface distances and rendering scores are not interchangeable. Verified >180° evidence does not establish a general accuracy guarantee for a 280° lens's rearward region.

**中。** 幾何推導及已實作方法均支持不經針孔影像的直接魚眼量測。已發表尺寸例子包含 Scaramuzza 的折反射棋盤重建及 Fu 等人的魚眼量測桿實驗。然而，重投影像素、軌跡公尺、表面距離與渲染分數不能互換。已核對的超過 180° 證據，尚不能建立 280° 鏡頭後向區域的通用精度保證。

## Online source papers / 線上原始論文

**EN.** These links point to author or publisher resources. The repository contains the generated research notes; third-party papers remain available at their online sources.

**中。** 以下連結指向作者或出版者資源；儲存庫保存已撰寫的研究筆記，第三方論文以線上來源提供。

- [Scaramuzza et al., ICVS 2006 / 全向校正與三維重建](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf)
- [Tezaur et al., 2022 / 非中心魚眼模型](https://openaccess.thecvf.com/content/CVPR2022W/OmniCV/papers/Tezaur_A_New_Non-Central_Model_for_Fisheye_Calibration_CVPRW_2022_paper.pdf)
- [Fu et al., 2014 draft / 多魚眼量測桿校正作者稿](https://arxiv.org/abs/1407.1267v1)
- [Lee & Civera, 2019 / 角度誤差三角交會](https://openaccess.thecvf.com/content_ICCV_2019/html/Lee_Closed-Form_Optimal_Two-View_Triangulation_Based_on_Angular_Errors_ICCV_2019_paper.html)
- [Wang et al., 2022 / LF-VIO 後向光線定位](https://arxiv.org/abs/2202.12613)
- [Perfetti et al., 2024 / Ant3D 狹窄空間度量](https://www.mdpi.com/1424-8220/24/13/4177)

## Interpretation key / 閱讀約定

**EN.** A reference label such as [KB06] refers to the source list in the same document, not the numbering of the original proposal. Equations with “proposed” or “derivation” describe analysis and experimental recommendations, not new empirical findings. Accuracy means agreement with a reference; repeatability concerns variation across repetitions. A full circular FoV is twice the maximum polar angle only under the stated symmetric convention.

**中。** [KB06] 等代號對應同一文件末尾的來源，不是原計畫書的文獻編號。標示「建議」或「推導」的公式屬於分析及實驗設計，不是新的實測發現。準確度表示與基準的一致性；重複性表示多次操作的變動。只有在所述對稱約定下，完整圓形 FoV 才是最大極角的兩倍。
