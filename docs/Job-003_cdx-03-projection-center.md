# 03 | Ray convergence, projection centers and non-central optics
# 03 | 視線匯聚、投影中心與非中心光學

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. Both standard models assume a center / 兩種標準模型都先假設中心

**EN.** Standard central KB and OCamCalib assign each pixel a direction through a common mathematical origin. Their object-space ray is Lp(λ)=C+λdp. Consequently, all modeled rays intersect at C by construction: fitting these models does not independently demonstrate that the physical lens has a single projection center. The calibrated image center or principal point is a two-dimensional parameter; it is not a measurement of a three-dimensional entrance-pupil position. A multi-camera rig can contain several individually central cameras while the rig as a whole is non-central.

**中。** 標準中心式 KB 與 OCamCalib 將每個像素指定為穿過同一數學原點的方向；物件空間光線為 Lp(λ)=C+λdp。因此模型中的光線依定義就交於 C：擬合這些模型，並不能獨立證明實體鏡頭具有單一投影中心。校正得到的影像中心或主點是二維參數，不是三維入瞳位置的量測。多相機系統可以由數個各自近似中心式的相機組成，但整體仍是非中心系統。

## 2. A physical lens may have viewpoint shift / 實體鏡頭可能具有視點偏移

**EN.** A non-central model uses Lp(λ)=op+λdp, allowing the ray origin to depend on the pixel. In an axial model, origins can vary along the optical axis as a function of incidence angle. Tezaur, Kumar and Nestares discuss entrance-pupil shift in real fisheye designs, with a Nikkor example changing by more than 1 cm across 0–90° incidence. This is lens-specific evidence, not a universal fisheye constant. Their real calibration comparison reduced average point-prediction error from 2.04 to 0.52 pixels when adding two viewpoint parameters to a six-parameter distortion model. It measures image prediction, not physical length error. [NC22, sections 2 and 4, Table 2]

**中。** 非中心模型採用 Lp(λ)=op+λdp，允許光線原點依像素而變。在軸向模型中，原點可以隨入射角沿光軸改變。Tezaur、Kumar 與 Nestares 討論真實魚眼設計的入瞳偏移；其中一個 Nikkor 例子在 0–90° 入射角範圍的變化超過 1 公分。這是特定鏡頭的證據，不是所有魚眼的固定常數。其真實校正比較中，在六參數畸變模型加入兩個視點參數後，平均點位預測誤差由 2.04 降至 0.52 像素；該數值衡量影像預測，而非實體長度誤差。[NC22，第 2、4 節及表 2]

## 3. Why low reprojection error is insufficient / 為何低重投影誤差不足以證明共點

**EN.** A central fit can partly absorb non-central effects into distortion coefficients or target poses, especially if all targets occupy a narrow distance range. The resulting calibration can interpolate well there but become biased at nearer objects or different depths. Adding polynomial degree changes the directional mapping but does not introduce per-pixel ray origins. To test centrality, use observations over several independently known depths and orientations, with some observations held out. A generalized model with many ray parameters needs substantially more data and regularization; parameter count alone does not guarantee better prediction.

**中。** 中心式擬合可能將部分非中心效應吸收到畸變係數或標靶姿態中，尤其當所有標靶都位於狹窄距離範圍時。這樣的校正在該區間可能插值良好，卻在較近物件或其他深度出現偏差。增加多項式階數會改變方向映射，但不會新增逐像素的光線原點。檢驗共點性時，應使用多個獨立已知的深度與方向，並保留部分觀測作驗證。含大量光線參數的廣義模型需要更多資料與正則化；參數數量本身不保證預測更好。

## 4. Estimate a best common center from measured rays / 從量測光線估計最佳共同中心

**EN.** The following is a proposed geometric diagnostic, not an experiment performed here. First estimate physical rays from a surveyed target or display moved through multiple known locations, while keeping the camera rigid. A ray must be supported by observations at separated depths; a single plane cannot determine its origin and direction independently. Fit or interpolate ray lines in a shared metric coordinate system, then find the least-squares common center C*=argmin Σ wi ||Pi(C−oi)||², where Pi=I−di diᵀ and each di is unit length. If A=ΣwiPi is well conditioned, C*=A⁻¹ΣwiPi oi. Pseudoinversion requires reporting unobservable directions rather than pretending they are accurately known.

**中。** 以下是建議的幾何診斷，並非本次已執行的實驗。首先固定相機，將經測量的標靶或顯示面移到多個已知位置，估計實體光線。每條光線需要不同深度的觀測支持；單一平面無法獨立決定光線原點與方向。在共同公制座標內擬合或插值光線後，求最小平方共同中心 C*=argmin Σ wi ||Pi(C−oi)||²，其中 Pi=I−di diᵀ，di 為單位方向。若 A=ΣwiPi 的條件良好，則 C*=A⁻¹ΣwiPi oi。若使用偽逆，必須說明不可觀測的方向，不能當作它們已被精確求出。

## 5. Define the “spread region” operationally / 明確定義「分散區域」

**EN.** For every independently estimated ray, report ei=||Pi(C*−oi)|| in millimetres: the perpendicular distance from the fitted center to that ray. Summarize weighted RMS, median, 95th percentile, maximum and their dependence on polar angle, azimuth and target distance. Also report the confidence ellipsoid of C* across repeated measurements. These are different objects: ray non-concurrence describes model mismatch; the confidence ellipsoid describes uncertainty in the estimated center. Neither is the optical point-spread function or a sensor blur diameter. Sliding oi along its own ray leaves ei unchanged, avoiding an arbitrary “origin cloud” definition.

**中。** 對每條獨立估計的光線，報告 ei=||Pi(C*−oi)||，單位為毫米，即擬合中心到該光線的垂直距離。彙整加權 RMS、中位數、第 95 百分位、最大值，以及它們隨極角、方位角與標靶距離的變化；另外報告重複量測所得 C* 的信賴橢球。這些是不同概念：光線不共點反映模型不符合程度，信賴橢球反映中心估計的不確定度。兩者都不是光學點擴散函數或感光元件的模糊直徑。將 oi 沿其光線滑動不會改變 ei，可避免任意定義「原點點雲」。

## 6. Connect centrality to the required measurement / 將共點性連到實際量測要求

**EN.** Center residuals alone do not determine 3D accuracy. Reconstruct held-out surveyed points and known lengths using the central and non-central calibrations under identical geometry. Report the change in metric bias and repeatability, not only training pixels. Near-range tasks are especially sensitive to an origin shift relative to object distance, whereas weak stereo intersection angles can dominate farther away. A central model is adequate if its independently verified error meets the application requirement; it need not be a physically perfect lens description. Standard KB versus standard OCamCalib is therefore primarily a comparison within the central assumption, not a direct test of the size of a physical convergence region.

**中。** 中心殘差本身不能決定三維精度。應以中心式及非中心式校正，在相同幾何條件下重建保留的已測量點與已知長度，報告公制偏差及重複性的改變，而非只看訓練像素。近距離任務對「原點位移相對物距」特別敏感；較遠處則可能由立體交會角不足主導誤差。只要獨立驗證誤差符合應用要求，中心模型便可能足夠，不必完全等同實體鏡頭。標準 KB 與標準 OCamCalib 的比較，主要是在共同中心假設內比較，而不是直接量測實體匯聚區域大小。

## Sources / 文獻來源

[NC22] Tezaur, Kumar & Nestares (2022), A New Non-central Model for Fisheye Calibration. CVPR Workshops. [Full paper / 全文](https://openaccess.thecvf.com/content/CVPR2022W/OmniCV/papers/Tezaur_A_New_Non-Central_Model_for_Fisheye_Calibration_CVPRW_2022_paper.pdf). 中文參考譯名：新的非中心魚眼校正模型。

[KB06] Kannala & Brandt (2006), A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses. [Author manuscript / 作者稿](https://users.aalto.fi/~kannalj1/calibration/Kannala_Brandt_calibration.pdf). 中文參考譯名：一般、廣角與魚眼鏡頭的通用相機模型及校正方法。

[OC06] Scaramuzza, Martinelli & Siegwart (2006), A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. [Paper / 論文](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf). 中文參考譯名：精確全向相機校正與運動恢復結構的彈性方法。

Further implementation resource / 延伸實作資源: Schöps et al., [Generic camera calibration author repository / 廣義相機校正作者倉庫](https://github.com/puzzlepaint/camera_calibration), supporting central and non-central generic models / 支援中心式與非中心式廣義模型。No numerical performance claim from this resource is used here. / 本文不引用此資源的數值效能結果。
