# 05 | Direct measurement without conversion to a pinhole image
# 05 | 不轉換為針孔影像的直接度量

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. The answer is yes / 答案是有

**EN.** A pinhole raster is not required to recover a calibrated bearing, triangulate points or optimize camera poses. Native fisheye measurement operates on observed pixels and their rays while retaining the fisheye projection function. Converting coordinates from pixels to unit vectors is a geometric computation, not resampling an image into a pinhole camera. This distinction is essential: a program can use normalized directions internally without producing or assuming a single perspective image.

**中。** 求校正視線、三角交會點或最佳化相機姿態，不需要先建立針孔影像。原生魚眼度量直接使用觀測像素與其光線，同時保留魚眼投影函數。將像素座標換成單位向量是幾何計算，不是將影像重取樣為針孔相機。這個區別很重要：程式可以在內部使用正規化方向，而不產生或假定單張透視影像。

## 2. A complete native-ray measurement chain / 完整的原生光線量測流程

**EN.** Calibrate the image-to-ray mapping, detect corresponding features in the raw images, and transform each bearing into a shared world frame: di=Ri ni. For central cameras, solve the intersection problem Xi=Ci+λi di across views. A practical initializer minimizes Σwi||(I−di diᵀ)(X−Ci)||². Refine X and, where appropriate, poses using native-pixel reprojection or angular residuals. Compute physical distances only after metric scale is fixed by a known baseline, surveyed controls or another valid scale source. Pure monocular reconstruction without such information remains ambiguous in scale.

**中。** 先校正像素到光線的映射，在原始影像偵測對應特徵，並將各視線轉到共同世界座標：di=Ri ni。對中心式相機，跨視角求解 Xi=Ci+λi di 的交會問題。實用的初值可藉由最小化 Σwi||(I−di diᵀ)(X−Ci)||² 取得，再使用原始像素重投影或角度殘差精化 X，必要時同時精化姿態。只有透過已知基線、測量控制點或其他有效尺度來源固定公制尺度後，才能計算實體距離。缺乏這些資訊的純單目重建仍有尺度歧義。

**EN.** Cheirality should be expressed as positive distance along the visible ray, λi>0. Requiring camera-coordinate z>0 would incorrectly discard valid rearward observations from a >180° model. For non-central cameras, replace Ci with the transformed pixel-dependent ray origin. The least-squares line intersection above is a geometric initializer, not a claim of statistical optimality for noisy pixels. Poorly intersecting or nearly parallel rays should produce a large uncertainty estimate or a rejection, not an apparently precise point.

**中。** 可見方向條件應表示為沿光線的距離為正，即 λi>0。若要求相機座標 z>0，便會錯誤剔除超過 180° 模型中有效的後向觀測。非中心式相機則將 Ci 換成經轉換、依像素而變的光線原點。上述最小平方直線交會是幾何初值，並不宣稱對含雜訊像素具有統計最優性。交會不良或近乎平行的光線，應產生大的不確定度或被拒絕，而不是輸出看似精確的點。

## 3. A model-independent triangulation paper / 不依賴針孔影像的三角交會文獻

**EN.** Lee and Civera derive closed-form globally optimal two-view triangulation for L1 and L∞ angular objectives. The inputs are calibrated rays and relative pose, so the formulation applies to central perspective, fisheye and omnidirectional cameras. This is direct evidence that triangulation does not inherently require pinhole rectification. Its optimality is tied to those angular objectives; it does not mean an arbitrary L2 pixel objective is solved optimally, or that calibration and relative pose are error-free. The paper supplies a geometric estimator, not a universal object-dimension tolerance. [TRI19]

**中。** Lee 與 Civera 推導了對 L1 及 L∞ 角度目標函數具有全域最優性的閉式雙視角三角交會。輸入為已校正光線及相對姿態，因此適用於中心式透視、魚眼與全向相機。這直接證明三角交會並不必然需要針孔校正影像。最優性僅對應這些角度目標函數；不代表任意 L2 像素目標也被最優求解，更不代表校正與相對姿態沒有誤差。該文提供幾何估計器，而不是通用物件尺寸公差。[TRI19]

## 4. Implemented systems and dimensional evidence / 已實作系統與尺寸證據

**EN.** Scaramuzza et al.'s ICVS 2006 paper already demonstrates this distinction: section 4.3 rectifies image regions as a visual calibration check, whereas section 5 reconstructs a trihedron from back-projected 3D vectors and two omnidirectional images. The presence of a rectification illustration does not mean the reconstruction requires a pinhole raster. Its mean checker-dimension error is 2.9 mm for 60 mm squares, using a catadioptric mirror camera; the scale and validation limitations are discussed in topic 04. [OC06]

**中。** Scaramuzza 等人的 ICVS 2006 原文已展示此區別：第 4.3 節把影像區域校正為透視圖，作為校正的視覺檢查；第 5 節則從兩張全向影像反投影的三維向量重建三面體。因此出現透視校正示意圖，不代表重建需要針孔影像。其折反射鏡相機對 60 毫米棋盤格報告平均尺寸誤差 2.9 毫米；尺度及驗證限制詳見主題 04。[OC06]

**EN.** ORB-SLAM3 includes native Kannala–Brandt fisheye support and uses projection/unprojection abstractions in its geometric processing, providing a practical route for pose and map estimation without first creating perspective images. Fu et al.'s wand method supplies a separate dimensional example: its angular camera model and multi-view reconstruction evaluate a 600 mm length, with 5.0890 mm RMS in its two-fisheye test. These are complementary findings: one demonstrates a deployed SLAM architecture, the other measures a physical length. Neither alone proves accuracy for all >180° rear-field pixels. [ORB21, WAND14]

**中。** ORB-SLAM3 原生支援 Kannala–Brandt 魚眼模型，並在幾何處理中使用投影／反投影介面，因此提供不先建立透視影像的姿態與地圖估計途徑。Fu 等人的量測桿方法則提供另一個尺寸例子：以角度相機模型及多視角重建評估 600 毫米長度，在雙魚眼測試中得到 5.0890 毫米 RMS。兩者是互補證據：前者展示已實作的 SLAM 架構，後者量測實體長度；任一者都不能單獨證明所有超過 180° 後向像素的精度。[ORB21, WAND14]

## 5. Distinguish three meanings of “direct” / 區分「直接」的三種意思

**EN.** Native-ray measurement avoids a pinhole raster. Spherical or equirectangular reprojection also avoids a pinhole raster, but still resamples the image and changes noise, area weighting and interpolation behavior. “Direct visual odometry” usually means optimizing image intensities instead of feature coordinates, and is a separate classification. A learned monocular depth map can likewise be predicted directly from fisheye input, but physical scale and out-of-distribution bias still need independent validation. Avoid treating these different uses of “direct” as equivalent metrological guarantees.

**中。** 原生光線度量避免建立針孔影像；球面或等距柱狀重投影也不使用針孔影像，但仍會重取樣，改變雜訊、面積權重及插值行為。「直接視覺里程計」通常指最佳化影像亮度，而不是特徵座標，屬於另一種分類。學習式單目深度圖也可直接由魚眼輸入預測，但實體尺度及分布外偏差仍需獨立驗證。不能把這些不同的「直接」等同於相同的度量保證。

## 6. Why a single pinhole view cannot retain the whole field / 為何單一針孔視圖不能保留整個視域

**EN.** Perspective radius is proportional to tanθ and diverges at θ=90°; one finite forward pinhole plane cannot preserve all directions of a >180° circular field. Multiple virtual views can cover more directions but introduce separate resampling and bookkeeping. Native bearings avoid that particular singular representation, while still requiring valid calibration, correspondence and visibility. For the proposal, a strong baseline is raw-pixel feature matching plus native-ray triangulation and fisheye bundle adjustment, evaluated against an otherwise identical multi-pinhole pipeline using held-out 3D checkpoints and lengths.

**中。** 透視半徑與 tanθ 成比例，在 θ=90° 發散；單一有限的前向針孔平面無法保留超過 180° 圓形視域的全部方向。多個虛擬視圖可以涵蓋更多方向，但會增加重取樣及資料管理。原生視線可避免這種特定表示方式的奇異點，仍需要有效校正、對應及可見性。對研究計畫，合適的基準是原始像素特徵匹配、原生光線三角交會及魚眼光束法平差，再與其他條件相同的多針孔流程比較，以保留的三維檢核點與長度評估。

## Sources / 文獻來源

[OC06] Scaramuzza, Martinelli & Siegwart (2006), A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. [Paper / 論文](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf). 中文參考譯名：精確全向相機校正與運動恢復結構的彈性方法。

[TRI19] Lee & Civera (2019), Closed-Form Optimal Two-View Triangulation Based on Angular Errors. ICCV. [Paper / 論文](https://openaccess.thecvf.com/content_ICCV_2019/html/Lee_Closed-Form_Optimal_Two-View_Triangulation_Based_on_Angular_Errors_ICCV_2019_paper.html). 中文參考譯名：基於角度誤差的閉式最優雙視角三角交會。

[ORB21] Campos et al. (2021), ORB-SLAM3: An Accurate Open-Source Library for Visual, Visual-Inertial, and Multimap SLAM. IEEE TRO. [Author record / 作者稿資訊](https://arxiv.org/abs/2007.11898). 中文參考譯名：適用視覺、視覺慣性與多地圖 SLAM 的精確開源函式庫。

[WAND14] Fu, Quan & Cai (2014), Calibration of Multiple Fish-Eye Cameras Using a Wand, v1. [Paper / 論文](https://arxiv.org/abs/1407.1267v1). 中文參考譯名：使用量測桿校正多部魚眼相機。
