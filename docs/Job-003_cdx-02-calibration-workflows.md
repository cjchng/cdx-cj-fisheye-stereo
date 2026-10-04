# 02 | Comparing KB and OCamCalib implementation workflows
# 02 | KB 與 OCamCalib 實施步驟的異同

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. Compare models, estimators and implementations separately / 分開比較模型、估計器與實作

**EN.** KB and Scaramuzza are camera-model families; OCamCalib is also a particular calibration toolbox. A fair comparison separates the ray model, the algorithm that estimates its parameters, and the software that evaluates projection. “OCamCalib uses two polynomials” describes a common runtime representation, not twice as many independent calibration experiments. Conversely, KB's compact radial model does not remove focal, principal-point, pose or optional asymmetry parameters. The procedure below is a recommended controlled comparison, rather than a claim that all original toolboxes perform identical steps. [KB06, OC06, OCtool]

**中。** KB 與 Scaramuzza 是相機模型家族；OCamCalib 同時也是特定的校正工具箱。公平比較應區分光線模型、參數估計演算法，以及執行投影的軟體。「OCamCalib 使用兩個多項式」描述常見的執行時表示方式，而不是需要兩倍的獨立校正實驗。另一方面，KB 的精簡徑向模型並沒有省去焦距、主點、姿態或選用的非對稱參數。以下流程是建議的控制比較，不是宣稱所有原始工具箱都執行完全相同的步驟。[KB06, OC06, OCtool]

## 2. Step 1: acquire comparable evidence / 步驟一：取得可比較的資料

**EN.** For both methods, lock focus and image processing, record image size/cropping, and use a dimensionally characterized target. Capture many target poses with changes in orientation, distance, image radius and azimuth. Reserve whole images and some distances for validation before fitting. For a full FoV above 180°, ensure that actual observations reach θ>90°; a large printed board seen only in front cannot validate rearward rays. Several target positions or a surveyed surrounding target field may be needed. Blurry, saturated or occluded corners should be rejected using the same rule for both methods.

**中。** 兩種方法都應固定焦距設定與影像處理，記錄影像尺寸及裁切方式，並使用尺寸已知且經檢驗的標靶。拍攝多種標靶姿態，涵蓋不同方向、距離、影像半徑與方位角。擬合前先保留完整影像及部分距離作驗證。完整 FoV 超過 180° 時，必須確保實際觀測到 θ>90°；只有前方的大棋盤不能驗證後向光線，可能需要移動標靶或使用經測量的環繞標靶場。模糊、過曝或遮蔽角點應以相同規則在兩種方法中剔除。

## 3. Step 2: initialize different parameterizations / 步驟二：初始化不同參數形式

**EN.** A common KB initialization starts near an equidistant lens and estimates intrinsics and each board pose; its radial law maps polar angle θ to image radius. OCamCalib instead estimates the polynomial f(ρ) describing a ray's axial component relative to its image-plane radius, together with center, affine terms and poses. Toolbox initialization and gauge conventions differ. Do not compare polynomial coefficients numerically across these models, and do not assume that equal polynomial order gives equal effective flexibility. Record the exact variant: KB4 versus an asymmetric original KB model, and OCam polynomial degree plus affine parameters. [KB06, OC06]

**中。** 常見 KB 初始化從接近等距鏡頭的設定出發，估計內參與各張標靶姿態；徑向關係將極角 θ 映射到影像半徑。OCamCalib 則估計 f(ρ)，用來描述光線的軸向分量與影像平面半徑的關係，同時估計中心、仿射項與姿態。工具箱的初始化及尺度約定並不相同。不能直接比較兩種模型的多項式係數，也不能假設相同階數就具有相同有效自由度。應記錄確切變體：例如 KB4 或含非對稱項的原始 KB，以及 OCam 多項式階數和仿射參數。[KB06, OC06]

## 4. Step 3: refine against the same measurements / 步驟三：以相同觀測精化

**EN.** A useful shared objective is the sum of robust, uncertainty-weighted native-pixel residuals pobs−π(TXtarget). Jointly refine intrinsics and target poses, but retain the same observations, weights and stopping criteria when comparing models. Better training residuals can simply result from extra parameters or compensation by board pose. Inspect residual vectors by radius, azimuth and target distance; repeated directional patterns suggest model bias, while a few isolated errors may indicate corner detection problems. Report parameter stability across repeated calibrations, not only the smallest achieved residual.

**中。** 合適的共同目標函數是原始像素殘差 pobs−π(TXtarget) 的穩健、依不確定度加權總和。可共同精化內參及標靶姿態，但比較模型時應維持相同觀測、權重與停止條件。訓練殘差較小可能只是因為參數更多，或由標靶姿態吸收了模型偏差。應依半徑、方位角與標靶距離檢視殘差向量；反覆出現的方向性樣式可能代表模型偏差，少數孤立誤差則可能來自角點偵測。除了最低殘差，也應報告重複校正的參數穩定性。

## 5. Step 4: build both mapping directions / 步驟四：建立雙向映射

**EN.** KB projection directly evaluates its angular polynomial; unprojection needs inversion, for example a bounded Newton solver or a validated lookup table. OCam unprojection directly evaluates f(ρ); projection either selects a physical polynomial root or evaluates a fitted g(α). Thus the direction that is naturally cheap is reversed between the two parameterizations. If g or a KB inverse table is used, measure its approximation error against a high-accuracy reference solver over the entire admitted domain, including boundaries and subpixel samples. Verify both pixel→ray→pixel and ray→pixel→ray consistency. [OCcode]

**中。** KB 投影直接代入角度多項式；反投影需要求逆，例如使用有界牛頓法或經驗證的查表。OCam 反投影直接代入 f(ρ)；投影則選取物理多項式根，或代入擬合的 g(α)。因此兩種參數形式天然較省運算的方向剛好相反。若採用 g 或 KB 反向查表，應在整個允許區間內與高精度求解器比較近似誤差，包含邊界及次像素取樣。必須同時驗證像素→光線→像素，以及光線→像素→光線的一致性。[OCcode]

## 6. Step 5: validate at three different levels / 步驟五：分三層驗證

**EN.** First test numerical consistency of the implemented functions; this can be excellent even for a physically wrong calibration. Second test held-out angular or reprojection accuracy against independent target observations. Third test the intended outcome: trajectory error, surveyed 3D checkpoints, or known lengths. Repeat the latter tests over distance and angle bins, with the same stereo baseline and reconstruction settings. A speed comparison should include projection, unprojection, Jacobians and failure rates; whether most time is spent detecting features or optimizing poses can matter more than polynomial evaluation alone. [DS18]

**中。** 第一層檢查程式函數的數值一致性；即使實體校正錯誤，這一層仍可能非常好。第二層以保留的獨立標靶觀測檢查角度或重投影精度。第三層驗證最終用途：軌跡誤差、經測量的三維檢核點，或已知長度。第三層應依距離與角度分組重做，維持相同立體基線及重建設定。速度比較應包含投影、反投影、雅可比矩陣與失敗率；整體時間是否主要花在特徵偵測或姿態最佳化，可能比多項式代入本身更重要。[DS18]

## 7. Implementation acceptance criteria / 實作驗收條件

**EN.** Require documented coordinate conventions, a valid-pixel mask, unique inversion in the admitted domain, stable axis limits, support for negative-z rays where claimed, and an explicit approximation tolerance. Preserve calibration covariance or repeat-calibration statistics for downstream uncertainty analysis. Choose the model that meets independent task accuracy with stable behavior and acceptable cost; neither the number of polynomials nor runtime inversion alone establishes a winner. A fair experiment may conclude that both central models reach a similar limit and that non-central optics, target errors or synchronization dominate.

**中。** 驗收應要求清楚的座標約定、有效像素遮罩、允許區間內唯一求逆、穩定的光軸極限處理、宣稱需要時對負 z 光線的支援，以及明確的近似容許值。應保存校正協方差或重複校正統計，以供下游不確定度分析。選擇能以穩定行為及可接受成本達到獨立任務精度的模型；多項式數量或是否即時求逆，都不能單獨決定優劣。公平實驗也可能發現兩種中心模型接近同一極限，而主要誤差來自非中心光學、標靶或同步。

## Sources / 文獻與程式來源

[KB06] Kannala & Brandt (2006), A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses. [Author manuscript / 作者稿](https://users.aalto.fi/~kannalj1/calibration/Kannala_Brandt_calibration.pdf). 中文參考譯名：一般、廣角與魚眼鏡頭的通用相機模型及校正方法。

[OC06] Scaramuzza, Martinelli & Siegwart (2006), A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. [Paper / 論文](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf). 中文參考譯名：精確全向相機校正與運動恢復結構的彈性方法。

[OCtool] [Author's OCamCalib tutorial / 作者工具箱教學](https://sites.google.com/site/scarabotix/ocamcalib-omnidirectional-camera-calibration-toolbox-for-matlab). [OCcode] [Author-attributed findinvpoly source mirror / 標示原作者的程式鏡像](https://github.com/nxp-imx/OCamCalib/blob/master/findinvpoly.m).

[DS18] Usenko, Demmel & Cremers (2018), The Double Sphere Camera Model. [Paper / 論文](https://arxiv.org/abs/1807.08957). 中文參考譯名：雙球相機模型。
