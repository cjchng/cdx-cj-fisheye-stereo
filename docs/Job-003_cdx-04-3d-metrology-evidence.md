# 04 | Literature on 3D positions and physical dimensions
# 04 | 三維位置與物件實體尺寸的量測文獻

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. Yes, but distinguish the measured quantities / 有相關文獻，但須區分量測對象

**EN.** Fisheye metrology literature includes calibrated stereo/multi-camera triangulation, photogrammetric bundle adjustment and comparison with surveyed references. Three outcomes must remain distinct: point-coordinate error ||Xhat−Xref||, length error Lhat−Lref, and surface distance after registration. A rigid translation can produce large coordinate error while preserving all lengths; a scale error changes lengths even when a similarity-aligned point cloud looks good. No single “accuracy” number describes all three. The examples below are selected primary-source evidence, not a meta-analysis or a controlled ranking of KB against OCamCalib.

**中。** 魚眼度量文獻包含經校正的立體／多相機三角交會、攝影測量光束法平差，以及與測量基準比較。必須區分三種結果：點座標誤差 ||Xhat−Xref||、長度誤差 Lhat−Lref，以及配準後的表面距離。剛體平移可能造成很大的座標誤差，卻保留所有長度；尺度誤差則會改變長度，即使經相似轉換對齊的點雲看起來很好。單一「精度」數字無法代表三者。以下為選取的第一手文獻證據，並非統合分析，也不是 KB 與 OCamCalib 的控制排名。

## 2. Direct dimensional evidence: a 600 mm wand / 直接尺寸證據：600 毫米量測桿

**EN.** Fu, Quan and Cai's 2014 draft calibrates multiple cameras using a moving one-dimensional wand and a generic angular camera model. Real fisheye lenses have nominal 185° FoV. After calibration, the authors place the wand at 20 positions in a 3×3×3 m volume and reconstruct its endpoints. For two fisheye cameras, Table 2 reports RMS error of the 600 mm end-to-end distance as 5.0890 mm for their method and 16.1052 mm for the Bouguet comparison. The first is approximately 0.85% of the reference length. A mixed three-camera configuration gives 3.9091 mm in Table 4. These are length errors, not reprojection pixels. [WAND14, section IV-B, equation 30]

**中。** Fu、Quan 與 Cai 的 2014 年稿件使用移動的一維量測桿及通用角度相機模型校正多相機；實驗魚眼鏡頭的標稱 FoV 為 185°。校正後，作者將量測桿放在 3×3×3 公尺體積中的 20 個位置，重建其端點。兩部魚眼相機的表 2 報告：600 毫米端點距離的 RMS 誤差，所提方法為 5.0890 毫米，Bouguet 比較方法為 16.1052 毫米；前者約為參考長度的 0.85%。混合三相機配置在表 4 得到 3.9091 毫米。這些是長度誤差，不是重投影像素。[WAND14，第 IV-B 節、式 30]

**EN.** This experiment uses the same type of wand for calibration and testing, though at new positions, and endpoints are manually extracted. The authors also identify target-manufacturing and image-localization limitations. It is useful dimensional evidence, but not fully independent traceable-gauge certification. The table does not report a KB stereo length result, so it cannot establish that the proposed method outperforms KB in metric reconstruction. Nor does nominal 185° coverage establish the error specifically for θ>90° rays.

**中。** 此實驗使用同類量測桿進行校正及測試，雖然測試位置不同，而且端點以人工擷取。作者也指出標靶製造與影像定位的限制。這是有用的尺寸證據，但並非完全獨立、可追溯標準器的認證。該表沒有提供 KB 的立體長度結果，因此不能據此證明所提方法的公制重建優於 KB。標稱 185° 視域也不能證明 θ>90° 光線區域的個別誤差。

## 2a. Scaramuzza's original dimensional test / Scaramuzza 原始尺寸測試

**EN.** The ICVS 2006 Scaramuzza paper itself includes a direct ray-based two-view reconstruction of a trihedron, using 135 manually matched points. For 6×6 cm checker squares it reports mean dimension error of 0.29 cm (2.9 mm); fitted plane angles are 94.6°, 86.8° and 85.3° instead of 90°. This is particularly relevant to the user's model comparison, but the physical camera is a hyperbolic-mirror catadioptric system, not a refractive fisheye lens. The short account does not establish an independent scale-calibration/validation separation; two-view monocular geometry alone cannot recover metric scale. Therefore this is a reported object-geometry demonstration, not evidence that every OCamCalib fisheye installation achieves 2.9 mm. [OC06, section 5]

**中。** Scaramuzza 的 ICVS 2006 原文即包含以光線直接進行的雙視角三面體重建，使用 135 個人工匹配點。對 6×6 公分棋盤格，報告平均尺寸誤差為 0.29 公分（2.9 毫米）；擬合平面夾角為 94.6°、86.8°、85.3°，而非 90°。這對使用者提出的模型比較尤其相關，但實體相機是雙曲面反射鏡組成的折反射系統，不是折射式魚眼鏡頭。其簡短敘述未建立獨立尺度校正與驗證的分離；單目雙視角幾何本身無法恢復公制尺度。因此這是文獻報告的物件幾何示範，不能解讀為所有 OCamCalib 魚眼配置都能達到 2.9 毫米。[OC06，第 5 節]

## 3. Field-scale evidence: Ant3D / 場域尺度證據：Ant3D

**EN.** Perfetti, Fassi and Vassena describe a five-camera fisheye surveying system for narrow spaces. In the San Vigilio field test, the reconstructed cloud is registered to laser-scanner data using the circular access room; the transformation is then applied to the full reconstruction. Selected extreme cross-sections differ by up to 4 cm, with 2.6 cm illustrated at the tower end. This demonstrates a practical architectural surveying outcome under that registration and acquisition geometry. It is not a global pointwise maximum computed over every surface, nor a guarantee for small manufactured components. [ANT24, section 4, Figures 19–20]

**中。** Perfetti、Fassi 與 Vassena 提出用於狹窄空間測量的五相機魚眼系統。在 San Vigilio 場域測試中，重建點雲先利用圓形入口房間與雷射掃描資料配準，再將該轉換套用到整體重建。選取的遠端截面差異最大約 4 公分，塔端示例為 2.6 公分。這說明在該配準與拍攝幾何下可達成實際建築測繪成果；它不是逐一比較所有表面所得的全域最大值，也不保證適用於小型製造零件。[ANT24，第 4 節、圖 19–20]

**EN.** Ant3D also reports calibration-marker object-space RMSE below 0.3 mm on average, with reference-coordinate accuracy around 0.2 mm. However, all markers are used as control points in that calibration test. These are fitting/control residuals, not an independent checkpoint result, and must not be transferred to the tunnel survey as a submillimetre accuracy claim. This distinction illustrates why the role of each reference point matters as much as its numerical residual. [ANT24, section 3.2]

**中。** Ant3D 也報告校正標記在物件空間的平均 RMSE 低於 0.3 毫米，參考座標精度約 0.2 毫米。然而，該校正測試中的所有標記都作為控制點使用。這是擬合／控制殘差，不是獨立檢核點結果，不能將其移用為隧道測量具有次毫米精度的主張。此差異顯示：參考點扮演的角色與殘差數值本身一樣重要。[ANT24，第 3.2 節]

## 4. Why range and baseline matter / 距離與基線為何重要

**EN.** For a simple well-conditioned stereo configuration, first-order depth uncertainty scales approximately as σZ≈Z² σα/B, where B is baseline and σα is disparity-angle uncertainty. This is a local approximation, not a universal fisheye bound. Increasing FoV can improve overlap and pose constraints but does not guarantee better triangulation angles for every object. For length L=||Xa−Xb||, propagate the joint endpoint covariance: Var(L)≈JΣabJᵀ. Correlation from shared calibration and pose matters; adding two independent endpoint variances can be misleading. These are analytical error-budget tools proposed here, not measured results from the cited experiments.

**中。** 對條件良好的簡化立體配置，一階深度不確定度約為 σZ≈Z² σα/B，其中 B 為基線、σα 為視差角不確定度。這是局部近似，不是普遍的魚眼誤差界限。擴大 FoV 可能增加重疊及姿態約束，但不保證每個物件都有更好的三角交會角。對長度 L=||Xa−Xb||，應傳遞兩端點的聯合協方差：Var(L)≈JΣabJᵀ。共同校正及姿態產生的相關性不可忽略；直接相加兩個獨立端點變異數可能誤導。這些是本文建議的解析誤差預算工具，不是所引實驗的實測結果。

## 5. A defensible measurement study / 可辯護的度量研究設計

**EN.** Use an independent surveyed 3D target field and several calibrated lengths, with near, middle and far distances and radial/azimuthal coverage. Keep validation targets out of calibration and final registration. Establish metric scale from a separately known baseline or controls; use only a rigid alignment if scale accuracy is being tested. Report signed length bias, RMS and maximum absolute length error, XYZ coordinate errors, repeatability and reference uncertainty. Separate model choice from feature localization, matching, baseline, temperature and synchronization. The defensible conclusion is that direct fisheye metrology is feasible, while achieved accuracy is task- and geometry-dependent; no reviewed evidence supplies a universal KB-versus-OCam millimetre ranking.

**中。** 應使用獨立測量的三維標靶場及多種經校驗長度，涵蓋近、中、遠距離和徑向／方位分布。驗證標靶不能參與校正或最終配準。以另外已知的基線或控制點建立公制尺度；若要檢驗尺度精度，僅能使用剛體對齊。報告有號長度偏差、RMS、最大絕對長度誤差、XYZ 座標誤差、重複性及參考值不確定度。模型選擇應與特徵定位、匹配、基線、溫度和同步分開分析。可辯護的結論是：直接魚眼度量可行，但精度取決於任務與幾何；本次文獻不能提供通用的 KB 對 OCam 毫米精度排名。

## Sources / 文獻來源

[OC06] Scaramuzza, Martinelli & Siegwart (2006), A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. [Paper / 論文](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf). 中文參考譯名：精確全向相機校正與運動恢復結構的彈性方法。

[WAND14] Fu, Quan & Cai (2014), Calibration of Multiple Fish-Eye Cameras Using a Wand, arXiv v1 draft. [Full record and PDF / 文獻及全文](https://arxiv.org/abs/1407.1267v1). 中文參考譯名：使用量測桿校正多部魚眼相機。Numerical results above refer to Tables 2 and 4 of this version. / 上述數值依此版本表 2、4。

[ANT24] Perfetti, Fassi & Vassena (2024), Ant3D—A Fisheye Multi-Camera System to Survey Narrow Spaces. Sensors 24, 4177. [Published article / 正式論文](https://www.mdpi.com/1424-8220/24/13/4177). 中文參考譯名：Ant3D：用於狹窄空間測量的魚眼多相機系統。
