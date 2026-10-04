# 06 | Evaluating trajectory accuracy with a 180° field of view
# 06 | 如何評估 180° 視域影像的軌跡精度

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. Two meanings of trajectory / 軌跡的兩種意義

**EN.** “Image trajectory” can mean the camera's motion through 3D space or a feature's path across successive images. These need different references and metrics. A low feature reprojection residual does not establish an accurate camera trajectory; bundle adjustment can distribute errors among poses, intrinsics and map points. Conversely, accurate camera positions do not prove that every peripheral pixel yields an accurate ray. A study should name the estimated variable before choosing an accuracy metric.

**中。** 「影像軌跡」可能指相機在三維空間中的運動，也可能指特徵在連續影像中的移動路徑；兩者需要不同基準及指標。特徵重投影殘差小，不代表相機軌跡準確，因為光束法平差可能將誤差分配到姿態、內參與地圖點。反之，相機位置準確，也不能證明每個周邊像素都有準確視線。研究應先指出估計的是哪個變數，再選擇精度指標。

## 2. A benchmark with independent pose reference / 具有獨立姿態基準的資料集

**EN.** The TUM VI benchmark provides synchronized fisheye stereo images and inertial measurements, with a motion-capture reference at 120 Hz. Images are 1024×1024 at 20 Hz and the IMU runs at 200 Hz. It is a relevant wide-angle trajectory benchmark, but should not be described as an exact, uniform 180° circular-field experiment without checking the particular calibration and crop. Ground-truth coverage also matters: many sequences have motion capture at their beginning and end rather than throughout the outdoor or corridor route. Evaluation must use only times with a valid reference. [TUM18]

**中。** TUM VI 提供同步魚眼立體影像與慣性量測，並有 120 Hz 動作捕捉基準。影像為 1024×1024、20 Hz，IMU 為 200 Hz。它是適合參考的廣角軌跡資料集，但未核對特定校正及裁切前，不應稱為精確且均勻的 180° 圓形視域實驗。真值涵蓋範圍也很重要：許多序列只在起點與終點具有動作捕捉，戶外或走廊的中段未必有完整基準。評估只能使用具有效參考值的時段。[TUM18]

## 3. What a published trajectory number actually means / 已發表軌跡數字的真正意義

**EN.** ORB-SLAM3 reports average accuracy of 9 mm for stereo-inertial operation on the TUM VI room sequences with fast handheld motion. This concerns estimated camera/IMU motion in those sequences and benefits from stereo and inertial information. It is not a 9 mm object-dimension result, a guarantee for long outdoor trajectories, or proof of accuracy in a separately isolated θ≈90° image band. The example shows that native fisheye trajectory estimation can be accurate under appropriate conditions, while the scope of the result remains essential. [ORB21, abstract and TUM VI evaluation]

**中。** ORB-SLAM3 報告，在 TUM VI 快速手持運動的 room 序列中，立體慣性模式平均精度達 9 毫米。這是這些序列的相機／IMU 運動估計結果，且利用了立體與慣性資訊；它不是物件尺寸的 9 毫米結果，不保證長距離戶外軌跡，也不是對 θ≈90° 環帶的獨立精度證明。此例說明在適當條件下，原生魚眼軌跡估計可以很準確，但結果適用範圍仍不可省略。[ORB21，摘要及 TUM VI 評估]

## 4. Camera-path metrics and alignment / 相機路徑指標與對齊

**EN.** Absolute trajectory error compares estimated and reference positions after one documented alignment. Report RMS, median, 95th percentile and maximum, plus orientation error. Relative pose error compares motion over a stated time or travelled-distance interval and reveals local drift that a global average can hide. For a metric stereo or visual-inertial method, normally use a rigid SE(3) alignment; a free-scale Sim(3) alignment can conceal scale error. For scale-ambiguous monocular output, Sim(3) may be appropriate, but report its fitted scale separately and do not claim verified metric scale. State whether alignment used all frames or only an initial segment.

**中。** 絕對軌跡誤差是在一次明確說明的對齊後，比較估計位置與參考位置；應報告 RMS、中位數、第 95 百分位、最大值及方向誤差。相對姿態誤差比較指定時間或行進距離間隔的運動，可揭露全域平均掩蓋的局部漂移。具有公制尺度的立體或視覺慣性方法通常應使用剛體 SE(3) 對齊；允許自由尺度的 Sim(3) 可能隱藏尺度誤差。對尺度不定的單目輸出，Sim(3) 可能合理，但應另報擬合尺度，不能宣稱已驗證公制尺度。也應說明對齊使用全部影格或僅初始片段。

## 5. Feature-track accuracy on the image or sphere / 影像或球面上的特徵軌跡精度

**EN.** For feature tracks, obtain ground truth from a controlled scene and independent pose/geometry, or a rendered sequence whose camera model is known. Compare observed feature positions with reference projections in pixels and compare bearings by εang=acos(clamp(nobs·nref,−1,1)). Report these errors by incidence angle and azimuth, together with track survival, incorrect associations and lost-track rate. Pixel error is not angularly uniform in a fisheye image: the local pixel-to-bearing Jacobian converts localization covariance into angular covariance. A stationary 3D feature generally traces a curve in the raw fisheye image; fitting a straight image line is not a universal correctness test.

**中。** 特徵軌跡的真值可來自具有獨立姿態／幾何的控制場景，或已知相機模型的渲染序列。以像素比較觀測特徵與參考投影，並以 εang=acos(clamp(nobs·nref,−1,1)) 比較視線方向。應依入射角與方位角報告誤差，同時記錄軌跡存活率、錯誤對應及失追率。魚眼影像的像素誤差不是均勻的角度誤差；可用局部像素到視線的雅可比矩陣將定位協方差轉為角度協方差。靜止三維特徵在原始魚眼影像中通常沿曲線移動，因此影像直線擬合不是通用的正確性測試。

## 6. A controlled 180° experiment / 控制 180° 實驗的建議

**EN.** Use the same raw recordings and a calibrated angular mask θ≤90° to define the 180° condition; a pixel-radius crop is equivalent only under the appropriate radial model and center. Compare KB and OCam under identical observations, matching, initialization and inertial input. Add angular bands, for example 0–60°, 60–80° and 80–90°, and test both fixed feature budget and all-available-feature settings. The first helps separate geometric benefit from feature count, while the second measures operational benefit. Report repeated runs, failures and total processed distance, not only successful trajectories. These bins and controls are proposed here, not a published standard.

**中。** 使用相同原始錄影，透過校正角度遮罩 θ≤90° 定義 180° 條件；只有在適當徑向模型與中心設定下，像素半徑裁切才與之等價。在相同觀測、匹配、初始化及慣性輸入下比較 KB 與 OCam。另設角度分組，例如 0–60°、60–80°、80–90°，並同時測試固定特徵數及使用全部可用特徵兩種設定。前者有助分離幾何效益與特徵數量，後者評估實際操作效益。應報告重複執行、失敗與總處理距離，而不只展示成功軌跡。這些分組與控制是本文建議，並非已發表標準。

## 7. Synchronization and reference uncertainty / 同步與參考值不確定度

**EN.** Transform ground truth to the same physical camera or IMU frame, including the lever arm and orientation offset. Estimate or verify time offset before evaluating fast motion; roughly, a timing error δt creates position discrepancy vδt and orientation discrepancy ωδt. Account for rolling shutter, exposure blur and the reference system's uncertainty. Distinguish accuracy against an external reference from repeatability across repeated runs. Neither smooth trajectories nor loop closure alone provide independent evidence of accuracy.

**中。** 必須將真值轉到相同的實體相機或 IMU 座標，包含槓桿臂與方向偏移。評估快速運動前應估計或驗證時間差；粗略而言，時間誤差 δt 會造成位置差 vδt 與方向差 ωδt。應納入滾動快門、曝光模糊及基準系統的不確定度。對外部基準的準確度與重複執行的重複性必須區分；軌跡平滑或成功閉環本身，都不是獨立精度證據。

## Sources / 文獻來源

[TUM18] Schubert et al. (2018), The TUM VI Benchmark for Evaluating Visual-Inertial Odometry. IROS. [Paper / 論文](https://arxiv.org/abs/1804.06120), [official data and evaluation information / 官方資料及評估資訊](https://cvg.cit.tum.de/data/datasets/visual-inertial-dataset). 中文參考譯名：評估視覺慣性里程計的 TUM VI 基準。

[ORB21] Campos et al. (2021), ORB-SLAM3: An Accurate Open-Source Library for Visual, Visual-Inertial, and Multimap SLAM. IEEE TRO. [Paper / 論文](https://arxiv.org/abs/2007.11898). 中文參考譯名：適用視覺、視覺慣性與多地圖 SLAM 的精確開源函式庫。
