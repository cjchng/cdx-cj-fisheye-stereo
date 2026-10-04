# 07 | Beyond 180°: restoration, trajectory and 3D accuracy
# 07 | 超過 180°：影像還原、軌跡與三維精度

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. There is evidence, but not one interchangeable accuracy claim / 有證據，但各種精度不能互換

**EN.** Literature reports calibration of >180° lenses, use of rearward rays in motion estimation, and native rendering of 200° fisheye images. These establish different capabilities. “Image restoration” may mean geometric remapping, novel-view rendering, or recovering a metric scene; each needs a different test. To substantiate rear-field 3D metrology, a study must identify observations with θ>90°, use independent metric references, and report error specifically for that region. A nominal lens FoV or a whole-image average does not satisfy all three conditions.

**中。** 文獻報告了超過 180° 鏡頭的校正、後向光線用於運動估計，以及 200° 魚眼影像的原生渲染；它們證明的是不同能力。「影像還原」可能指幾何重映射、新視角渲染，或恢復公制場景，各自需要不同測試。要支持後半球三維度量，研究必須指出 θ>90° 的觀測、使用獨立公制基準，並單獨報告該區域誤差。標稱鏡頭 FoV 或全影像平均，不能同時滿足這三項條件。

## 2. Calibration evidence is not a rear-field metric benchmark / 校正證據不等於後半球公制基準

**EN.** The original KB paper's 190° experiment demonstrates that KB is not inherently limited to 180°. Its validation is in reprojection pixels, not object length. Likewise, the 185° wand experiment in topic 04 measures physical lengths but does not isolate which measurements are supported by rearward pixels. These are useful complementary results, yet neither supplies a θ>90°-only coordinate-error distribution. Full FoV=190° reaches θmax=95°, which is also much less demanding in angular coverage than a proposed 280° lens reaching θmax=140°. [KB06, WAND14]

**中。** KB 原始論文的 190° 實驗證明 KB 本身並不受限於 180°，但其驗證單位是重投影像素，而不是物件長度。主題 04 的 185° 量測桿實驗雖然測量實體長度，卻沒有分離哪些結果由後向像素支持。兩者提供互補證據，但都沒有提供僅針對 θ>90° 的座標誤差分布。完整 FoV=190° 只到 θmax=95°，其角度覆蓋要求也遠低於 280° 鏡頭的 θmax=140°。[KB06, WAND14]

**EN.** Scaramuzza's ICVS 2006 experiment is another historical example: it reports vertical coverage above 200° for a catadioptric camera and reconstructs a known trihedron. However, it does not report the checker-size error separately for rearward angular bins. A >200° camera plus an object reconstruction is therefore relevant evidence, but still does not establish a rear-field-only dimensional tolerance. [OC06]

**中。** Scaramuzza 的 ICVS 2006 實驗也是歷史例子：它報告折反射相機的垂直視域超過 200°，並重建已知三面體；但沒有針對後向角度分組另報棋盤格尺寸誤差。因此，「超過 200° 的相機加上物件重建」是相關證據，仍不足以建立僅針對後半球的尺寸公差。[OC06]

## 3. Explicit rearward-ray evidence: LF-VIO / 明確的後向光線證據：LF-VIO

**EN.** LF-VIO evaluates a panoramic annular camera covering 360° azimuth and polar angles 40–120°, including negative-z directions. This is an annulus with a central blind region, not a circular 240° fisheye cap. Its PALVIO experiment uses motion capture and compares angular subsets. In Table II, sequence ID06 has absolute trajectory error of 0.093 m for the full tested field versus 0.124 m for the 40–90° subset. This supports the usefulness of rearward observations in that visual-inertial system; it does not measure the dimensional accuracy of objects seen exclusively behind the camera. [LF22, section IV-C, Table II]

**中。** LF-VIO 評估方位角 360°、極角 40–120° 的全景環形相機，包含負 z 方向。這是具有中央盲區的環形視域，不是完整 240° 圓形魚眼球冠。其 PALVIO 實驗使用動作捕捉並比較不同角度子集。在表 II，ID06 序列使用完整測試視域的絕對軌跡誤差為 0.093 公尺，僅使用 40–90° 子集則為 0.124 公尺。這支持後向觀測對該視覺慣性系統的效益，卻沒有量測完全位於相機後方物件的尺寸精度。[LF22，第 IV-C 節、表 II]

## 4. Native 200° rendering evidence / 原生 200° 渲染證據

**EN.** Gunes et al.'s August 2025 v1 evaluates Fisheye-GS and 3DGUT with real 200° imagery and compares 200°, 160° and 120° conditions. Fisheye-GS benefits from reducing FoV to 160°, whereas 3DGUT remains more stable at 200°. Evaluation uses held-out views and PSNR, SSIM and LPIPS: these are appearance metrics. They do not establish millimetre geometry, calibrated physical scale, or an independent error bound for the 90–100° annulus. The study provides evidence of rendering feasibility; it must not be recast as certified object-space measurement. This note cites v1 explicitly because the current arXiv record has a revised title. [GS25v1]

**中。** Gunes 等人的 2025 年 8 月 v1 使用真實 200° 影像評估 Fisheye-GS 與 3DGUT，並比較 200°、160°、120° 條件。Fisheye-GS 在縮小到 160° 後受益，而 3DGUT 在 200° 下較穩定。評估使用保留視角與 PSNR、SSIM、LPIPS，這些都是外觀指標，不能證明毫米級幾何、已校驗實體尺度，或 90–100° 環帶的獨立誤差界限。該研究提供渲染可行性證據，不應被改述為經認證的物件空間度量。由於目前 arXiv 記錄已更改標題，本文明確引用 v1。[GS25v1]

## 5. FIORD: useful data, with important evaluation limits / FIORD：有用資料及其評估限制

**EN.** FIORD supplies indoor/outdoor fisheye imagery and laser-scanner reference data, making it relevant to future metric evaluation. However, section 3.5 of the reviewed manuscript rectifies images into a pinhole-compatible format for its baseline rendering experiments. Section 3.4 describes scale-inclusive alignment using 7–10 correspondences and gives a 25 cm alignment RMSE example for the largest indoor scene. That is an alignment statistic, not a rear-annulus reconstruction error or universal ground-truth accuracy. Before claiming fine metric performance on this dataset, quantify registration uncertainty and use independent checks. [FIORD25]

**中。** FIORD 提供室內外魚眼影像及雷射掃描基準，有助未來的公制度量評估。然而，本次查閱稿件第 3.5 節將影像校正為針孔相容格式，用於基準渲染實驗；第 3.4 節則利用 7–10 組對應點進行包含尺度的對齊，並對最大室內場景舉出 25 公分配準 RMSE。那是配準統計，不是後半球環帶重建誤差，也不是普遍的真值精度。若要利用此資料集主張精細公制效能，必須先量化配準不確定度，並使用獨立檢核。[FIORD25]

## 6. What has not been established here / 本次尚未證實的部分

**EN.** In the sources verified for this review, no study establishes a general dimensional tolerance for the θ=90–140° region of a single 280° lens, nor a controlled KB-versus-OCam accuracy ranking there. This is a bounded literature finding, not proof that no such paper exists. A dual-lens 360° camera does not automatically validate one lens over 280°; nominal coverage, sensor cropping, lens obstruction and calibrated angular coverage differ. The proposal should frame rear-field metrology as a question to test rather than an already demonstrated consequence of model support.

**中。** 在本次核對的來源中，尚未有研究建立單一 280° 鏡頭 θ=90–140° 區域的通用尺寸公差，也沒有在該區域控制比較 KB 與 OCam 精度。這是有範圍限制的文獻查核結果，不是證明世上不存在相關論文。雙鏡頭 360° 相機不會自動驗證單鏡頭的 280° 能力；標稱覆蓋、感光元件裁切、鏡體遮蔽與已校正角度範圍都不同。研究計畫應將後半球度量視為待檢驗問題，而不是模型支援後便已證明的結果。

## 7. A concrete rear-field validation protocol / 具體的後半球驗證程序

**EN.** Place surveyed 3D targets and certified lengths across θ bins 0–60°, 60–85°, 85–95°, 95–110° and then up to the actual calibrated limit, with several azimuths and distances. These are suggested bins; adjust them to sample availability. Acquire stereo or multi-view overlap so rearward targets have non-degenerate intersections. Compare front-only, rear-only and mixed observations with fixed and unrestricted feature budgets. Keep validation controls out of calibration and registration. Report bearing error, independent XYZ errors, length bias/RMS/P95, completeness and failure rate per bin. Also report how much of each bin is genuinely observed rather than interpolated.

**中。** 在 θ 分組 0–60°、60–85°、85–95°、95–110°，以及更外側直到實際校正極限，配置經測量的三維標靶及經校驗長度，涵蓋多個方位與距離。這些是建議分組，應依實際取樣調整。拍攝具有立體或多視角重疊的資料，確保後向標靶有非退化交會。比較前向限定、後向限定及混合觀測，並分別使用固定及不限特徵數。驗證控制資料不可參與校正或配準。逐組報告視線角度誤差、獨立 XYZ 誤差、長度偏差／RMS／P95、完整率與失敗率，也要說明每組實際觀測覆蓋與插值部分的比例。

**EN.** For geometric image remapping, compare rays or independently projected fiducials, and state the output projection and sampling density. For novel-view synthesis, report angularly masked appearance scores as well as whole-image scores; when comparing FoVs, include the same common angular region. For 3D metrology, retain metric units and independent scale checks. A successful result in one evaluation track should not substitute for the other two. This protocol is a proposed research design; no new calibration or reconstruction experiment was performed for these documents.

**中。** 幾何影像重映射應比較光線或獨立投影標記，並說明輸出投影及取樣密度。新視角合成應同時報告角度遮罩內及全影像外觀分數；比較不同 FoV 時，也應比較相同的共同角度區域。三維度量則保留公制單位及獨立尺度檢核。任何一類評估成功，都不能替代其他兩類。以上為建議研究設計；本次文件未執行新的校正或重建實驗。

## Sources / 文獻來源

[OC06] Scaramuzza, Martinelli & Siegwart (2006), A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. [Paper / 論文](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf). 中文參考譯名：精確全向相機校正與運動恢復結構的彈性方法。

[KB06] Kannala & Brandt (2006), A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses. [Author manuscript / 作者稿](https://users.aalto.fi/~kannalj1/calibration/Kannala_Brandt_calibration.pdf). 中文參考譯名：一般、廣角與魚眼鏡頭的通用相機模型及校正方法。

[WAND14] Fu, Quan & Cai (2014), Calibration of Multiple Fish-Eye Cameras Using a Wand. [v1 paper / v1 論文](https://arxiv.org/abs/1407.1267v1). 中文參考譯名：使用量測桿校正多部魚眼相機。

[LF22] Wang et al. (2022), LF-VIO: A Visual-Inertial-Odometry Framework for Large Field-of-View Cameras with Negative Plane. [Paper / 論文](https://arxiv.org/abs/2202.12613). 中文參考譯名：支援負深度平面之大視域相機的視覺慣性里程計框架。

[GS25v1] Gunes et al. (2025), Evaluating Fisheye-Compatible 3D Gaussian Splatting Methods on Real Images Beyond 180° Field of View. [Version-specific full text / 指定版本全文](https://arxiv.org/html/2508.06968v1). 中文參考譯名：以超過 180° 的真實影像評估魚眼相容三維高斯潑濺方法。

[FIORD25] Gunes et al. (2025), FIORD: A Fisheye Indoor-Outdoor Dataset with LIDAR Ground Truth for 3D Scene Reconstruction and Benchmarking. [Paper / 論文](https://arxiv.org/abs/2504.01732). 中文參考譯名：具雷射真值的室內外魚眼三維重建基準資料集。
