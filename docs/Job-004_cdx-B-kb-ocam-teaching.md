# 教材 B：從光線幾何理解 KB 與 OCam
# Handout B: Understanding KB and OCam through ray geometry

課堂講義 / Classroom handout · 2026-09-29 · 逐段中英對照 / Paragraph-paired bilingual edition

**中。** 本教材依本次討論重組，先定義影像與光線，再比較參數化、校正、求逆及度量。本文的 KB 指 Kannala–Brandt 的角度多項式模型；OCam 指 Scaramuzza 的多項式模型及 OCamCalib 工具的典型實作，兩者不能一概視為所有同名軟體的完整功能。公式旁附符號及適用條件，文末提供原始來源。

**EN.** This handout reorganizes our discussion around pixels and rays, then parameterization, calibration, inversion, and measurement. KB denotes the Kannala–Brandt angular polynomial model; OCam denotes Scaramuzza’s polynomial model and typical OCamCalib implementations. These descriptions do not cover every feature of every implementation using those names. Equations include explanations and assumptions; original sources appear at the end.

## 1. 共同起點：像素決定一條光線 / The shared starting point: a pixel determines a ray

**中。** 經過校正，一個像素通常可以轉成單位方向向量 n。向量是三個座標組成的箭頭；「單位」表示箭頭長度為 1，只保留方向。在中心投射模型中，物件點 X 位於下式所描述的射線。C 是相機投射中心，R 是把相機方向轉到物件座標的旋轉矩陣，λ 是尚未知道的正距離。單張像素沒有提供 λ，因此「影像到物件空間」通常是得到射線，而非唯一三維點。KB 與 OCam 都描述這個映射。[B1](#ref-b1)、[B2](#ref-b2)

**EN.** After calibration, a pixel generally determines a unit direction vector n: an arrow represented by three coordinates, scaled to length 1. In a central model, the object point X lies on the ray below. C is the projection center, R rotates a camera direction into object coordinates, and λ is an unknown positive distance. A single pixel does not determine λ, so image-to-object-space mapping normally yields a ray, not a unique 3D point. Both KB and OCam describe this mapping. [B1](#ref-b1), [B2](#ref-b2)

$$
\mathbf X=\mathbf C+\lambda\mathbf R\mathbf n,\qquad \lambda>0 \qquad\mathrm{(B1)}
$$

**中。** 「投影」把已知三維方向轉成影像座標；「反投影」把像素轉成光線。反投影不是恢復完整三維物件。真正的深度需要另外的條件，例如雙目基線、已知平面、已知物件尺寸或其他距離感測。公式中的旋轉不會改變長度，因此 λ 可以解釋成距離。

**EN.** Projection maps a known 3D direction to image coordinates; unprojection maps a pixel to a ray. Unprojection does not reconstruct the entire object. Depth requires additional information, such as a stereo baseline, a known plane, a known object dimension, or another range sensor. Rotation preserves length, which lets λ represent distance.

## 2. 符號與角度：先統一座標 / Symbols and angles: establish one convention

**中。** 先移除影像中心及仿射變形，得到校正平面座標 u、v；這裡不是感測器未處理的列、行編號。ρ 是中心到該點的半徑。對方向向量 (x,y,z)，r 是橫向分量的長度，θ 是相對正 z 光軸的偏軸角，φ 是繞光軸的方位角。下文固定正 z 為前方，使用弧度。atan2(r,z) 同時考慮兩個分量的正負，能區分前方與後方，θ 的範圍是 0 至 π。

**EN.** Remove the image center and affine deformation first to obtain corrected coordinates u and v; these are not raw sensor row and column indices. The radius from the corrected center is ρ. For a direction (x,y,z), r is its transverse magnitude, θ is the angle from the positive z optical axis, and φ is azimuth around that axis. We use positive z as forward and angles in radians. The two-argument function atan2(r,z) distinguishes front and rear directions, giving θ from 0 to π.

$$
\rho=\sqrt{u^2+v^2},\quad r=\sqrt{x^2+y^2},\quad \theta=\operatorname{atan2}(r,z),\quad \alpha=\frac{\pi}{2}-\theta \qquad\mathrm{(B2)}
$$

| 符號 / Symbol | 意義 / Meaning | 注意 / Caution |
|---|---|---|
| ρ | 校正影像半徑 / Corrected image radius | 單位取決於座標定義 / Units depend on coordinates |
| θ | 偏軸角 / Angle from optical axis | 90° 是赤道方向 / 90° is the equator |
| α | 相對橫向平面的仰角 / Elevation above transverse plane | α 與 θ 不是同一角 / Not the same angle as θ |
| P(θ) | KB 徑向函數 / KB radial function | 輸出 ρ / Outputs ρ |
| f(ρ) | OCam 軸向分量函數 / OCam axial-component function | 不是物件深度 / Not object depth |
| C、n | 光線起點、方向 / Ray origin and direction | 中心模型共用 C / A central model shares C |

**中。** 對繞光軸對稱的完整圓形視野，220° 全視野對應 θ 最大 110°；全視野 180° 的邊緣才是 θ=90°。不能把「180° 全視野」當作偏軸角 θ=180°。某些環形全景相機另外使用 360° 方位角及一段偏軸角範圍，也不能用兩倍最大偏軸角概括其有效視野。

**EN.** For a symmetric circular field, a 220° full field corresponds to a maximum θ of 110°. The edge of a 180° full field is θ=90°, not θ=180°. Annular panoramic cameras may instead specify 360° azimuth and a restricted polar-angle band; twice their maximum polar angle does not describe their valid field.

## 3. 實驗資料與自變數 / Experimental data and independent variables

**中。** 您提出「兩者可以共用不同視角的棋盤影像」是合理的：已知棋盤角點座標及偵測到的像素，可作為兩種模型的共同輸入。然而自變數是函數的輸入角色，不代表它必定能直接量得。KB 的 θ 通常由目前估計的棋盤姿態算出；OCam 的 ρ 也依賴正在估計的影像中心與仿射參數。兩者均會有直接觀測量與待估參數。棋盤位置需要充分分布並有姿態變化，但不等同於要求影像之間具有一般 SfM 的場景重疊條件。

**EN.** Your proposal to use the same checkerboard images from different viewpoints is sound: known target coordinates and detected image corners can serve both models. However, an independent variable is a function’s input, not necessarily a directly measured quantity. KB’s θ is usually computed from an estimated target pose; OCam’s ρ also depends on the estimated image center and affine parameters. Both therefore combine observations with unknowns. Calibration benefits from broad image coverage and varied poses, which is different from requiring general scene overlap as in structure from motion.

**中。** 假設棋盤點是 Xₜ，目前姿態是旋轉 Rₜ 與平移 tₜ，相機座標就是 X_c=RₜXₜ+tₜ。從 X_c 的三個分量計算式 B2 的 θ。因為姿態與鏡頭參數相互影響，實作通常先初始化，再共同改善參數；不是先完全知道每個 θ 才開始校正。

**EN.** If a target point is Xₜ and its current pose consists of rotation Rₜ and translation tₜ, its camera coordinates are X_c=RₜXₜ+tₜ. Their components supply θ in Equation B2. Since pose and lens parameters interact, implementations generally initialize and then refine them together; all angles need not be known exactly before calibration begins.

## 4. KB：以偏軸角產生影像半徑 / KB: image radius as a function of polar angle

**中。** KB 原始論文的基本徑向模型使用 θ 的奇次冪。下式的 P 表示一個多項式函數；k₁ 至 k₅ 是要估計的係數，θ³ 表示 θ 乘以自己三次。基底是 θ、θ³、θ⁵、θ⁷、θ⁹，而不只是「θ」。奇次形式與將徑向關係延伸為奇函數的選擇有關。原始論文另有非對稱修正，因此不能把整篇 KB 方法完全縮成此一徑向式。[B1](#ref-b1)

**EN.** The basic radial model in the original KB paper uses odd powers of θ. P denotes a polynomial; k₁ through k₅ are estimated coefficients, and θ³ means θ multiplied by itself three times. The basis is θ, θ³, θ⁵, θ⁷, θ⁹, rather than simply “θ.” The odd form reflects an odd extension of the radial relation. The original paper also includes asymmetric corrections, so this radial equation is not its entire model. [B1](#ref-b1)

$$
\rho=P(\theta)=k_1\theta+k_2\theta^3+k_3\theta^5+k_4\theta^7+k_5\theta^9 \qquad\mathrm{(B3)}
$$

**中。** 得到 ρ 後，以方位角 φ 決定影像點的方向：u=ρ cosφ、v=ρ sinφ，再套回像素尺度、中心等參數。常見「KB4」實作將一次項正規化為 θ，另估四個高次係數與焦距尺度；這種係數記法不可直接與式 B3 的 k₁ 至 k₅ 對照。若反投影，先從像素求 ρ，再解 P(θ)=ρ，最後用下式形成單位方向。

**EN.** Once ρ is known, azimuth gives u=ρ cosφ and v=ρ sinφ, followed by pixel scaling and the image-center transformation. Common “KB4” implementations normalize the first-order term to θ and estimate four higher-order coefficients plus focal scales; their coefficient notation is not interchangeable with k₁ through k₅ in Equation B3. Unprojection recovers ρ, solves P(θ)=ρ, then constructs the unit direction below.

$$
\mathbf n=(\sin\theta\cos\phi,\ \sin\theta\sin\phi,\ \cos\theta) \qquad\mathrm{(B4)}
$$

## 5. OCam：以影像半徑產生軸向分量 / OCam: axial component as a function of image radius

**中。** Scaramuzza 的一般表示先算 f(ρ)，再組合未正規化向量 q。係數 a₀、a₁……aₙ 對應基底 1、ρ、ρ²……ρᴺ；特定模型、初始化或工具版本可能對某些係數施加限制。N 是最高次數。||q|| 表示向量長度，除以它就得到單位方向。不同影像中心、軸方向及係數排列慣例必須依原文與程式確認。[B2](#ref-b2)、[B4](#ref-b4)

**EN.** Scaramuzza’s general representation evaluates f(ρ), then assembles the unnormalized vector q. Coefficients a₀, a₁, …, aₙ multiply the basis 1, ρ, ρ², …, ρᴺ; particular models, initialization procedures, or tool versions may constrain some coefficients. N is the highest degree. The notation ||q|| means vector length; dividing by it yields a unit direction. Image centers, axis signs, and coefficient ordering must be checked against the source and implementation. [B2](#ref-b2), [B4](#ref-b4)

$$
f(\rho)=\sum_{j=0}^{N}a_j\rho^j,\quad \mathbf q=(u,v,f(\rho)),\quad \mathbf n=\frac{\mathbf q}{\|\mathbf q\|} \qquad\mathrm{(B5)}
$$

**中。** Σ 是「把各項加起來」的簡寫，其中 ρ⁰=1。f(ρ) 是表示光線所用的軸向分量，不是物件到相機的距離，也不是隨像素改變的實體焦距。改變 q 的正比例尺度不會改變方向；因此不要把 q 的每個分量直接當作單位球面的座標。

**EN.** Σ means “add the terms,” with ρ⁰=1. The value f(ρ) is an axial component used to represent a ray, not object distance or a physical focal length varying by pixel. Multiplying q by a positive scale leaves its direction unchanged; its components therefore should not be mistaken for coordinates on the unit sphere.

## 6. 為何仍是不同模型？ / Why are they still different models?

**中。** 您的核心整理可以保留：兩者都以實驗資料估計有限次多項式的係數，也都可以表達同一種中心光線幾何。差異在於它們把「不同的量」限制成有限次多項式。KB 限制 ρ 對 θ 的關係；OCam 限制 q 的軸向分量對 ρ 的關係。這不只是把相同多項式從單項式基底換成 Chebyshev 基底；後者在同次數下可描述相同的多項式空間，而這裡改變了函數輸出與輸入間的幾何關係。

**EN.** Your main synthesis remains valid: both estimate finite polynomial coefficients from experiments and can describe the same central ray geometry. Their difference is which quantity is constrained to be polynomial. KB constrains ρ as a function of θ; OCam constrains the axial ray component as a function of ρ. This is more than replacing monomials with a Chebyshev basis for the same polynomial space: the geometric relationship between the function’s input and output changes.

$$
f(\rho)=\rho\cot\!\left(P^{-1}(\rho)\right) \qquad\mathrm{(B6)}
$$

**中。** 式 B6 假設兩者採相同座標尺度、中心模型及單調可逆的徑向映射。P⁻¹ 是「由 ρ 找回 θ 的反函數」，不是 1/P；cotθ=cosθ/sinθ。它來自軸向與橫向分量的比值 f/ρ=cosθ/sinθ。即使 P 是有限次多項式，這個 f 通常也不是有限次多項式。因而同樣階數或參數數目的 KB 與 OCam 通常不是完全等價的函數族。

**EN.** Equation B6 assumes matching coordinate scales, a central model, and an invertible monotonic radial mapping. P⁻¹ means the inverse function recovering θ from ρ, not 1/P; cotθ=cosθ/sinθ. The equation follows from the axial-to-transverse ratio f/ρ=cosθ/sinθ. Even when P is a finite polynomial, the corresponding f generally is not. KB and OCam with the same degree or parameter count therefore do not generally define identical function families.

$$
P(\theta)=F\theta\ \Longrightarrow\ f(\rho)=\rho\cot(\rho/F)=F-\frac{\rho^2}{3F}-\frac{\rho^4}{45F^3}-\cdots \qquad\mathrm{(B7)}
$$

**中。** 這個例子使用理想等距投射，F 是把弧度角轉成影像長度的尺度。它在 KB 中只需一次項，但對應的 OCam 函數一般要無窮級數才能精確表示；有限項只能近似。式中的省略號表示還有更高次項，不能拿低次截斷式保證大視角準確度。一般校正得到的最小平方擬合係數也不必等於在光軸處求導得到的 Taylor 係數。

**EN.** This example uses ideal equidistant projection, with F converting radians to image length. KB needs only its linear term, whereas the corresponding OCam function generally requires an infinite series for an exact representation. A finite truncation is an approximation; the omitted higher-order terms prevent a low-order truncation from guaranteeing wide-angle accuracy. Least-squares calibration coefficients also need not equal Taylor coefficients obtained from derivatives at the optical axis.

## 7. 求逆與第二個多項式 / Inversion and the second polynomial

**中。** KB 反投影常用 Newton 法：從初始 θ 開始，根據目前半徑誤差及曲線斜率修正角度。P′ 是 P 對 θ 的導數，也就是局部斜率；下標 m 是第幾次修正。斜率太小、初始值不佳或走出校正範圍都可能造成失敗。可用區間保護、二分法或預先建立的查表改善穩健性。低階特殊模型可能直接可逆，因此「KB 必定要迭代」並不成立。

**EN.** KB unprojection often uses Newton’s method: starting from an initial θ, it updates the angle using the radius error and the local slope. P′ is the derivative, or local slope; subscript m counts updates. A small slope, poor initialization, or departure from the calibrated domain can cause failure. Bracketing, bisection, or a precomputed lookup table can improve robustness. Special low-order models may have a direct inverse, so KB does not mathematically require iteration in every implementation.

$$
\theta_{m+1}=\theta_m-\frac{P(\theta_m)-\rho}{P'(\theta_m)} \qquad\mathrm{(B8)}
$$

**中。** OCam 的像素到光線可直接算式 B5，但光線到像素一般需要反解 f(ρ)=(z/r)ρ，其中 r>0。快速版本再擬合一個由仰角 α 到半徑 ρ 的多項式 g。這是用已校正模型產生樣本後建立的計算近似，不必解讀為兩次互相獨立的物理校正。所查閱的 findinvpoly 程式以取樣點上的像素誤差選擇階數；這種數值近似誤差不是三維量測精度。[B4](#ref-b4)

**EN.** OCam evaluates pixel-to-ray mapping directly with Equation B5, but ray-to-pixel mapping generally requires solving f(ρ)=(z/r)ρ for r>0. A fast version fits another polynomial g mapping elevation α to radius ρ. This is a computational approximation constructed from samples of the calibrated model, not necessarily a second independent physical calibration. The inspected findinvpoly implementation selects degree using pixel error at sampled points; that approximation error is not a 3D measurement accuracy. [B4](#ref-b4)

**中。** Double Sphere 第 2.4 節所說的兩個多項式與不同角度定義，應在上述層次理解：它比較的是特定 Scaramuzza 表示與計算方式。不能由這句話推論 KB 沒有反投影，也不能推論 OCam 的兩個多項式各自描述不同物理鏡頭。[B3](#ref-b3)

**EN.** The two polynomials and different angle definitions discussed in Double Sphere Section 2.4 should be understood at this level: they concern a particular Scaramuzza representation and evaluation strategy. They do not imply that KB lacks unprojection or that OCam’s two polynomials represent separate physical lenses. [B3](#ref-b3)

## 8. 負軸向分量與超過 180° / Negative axial components and fields beyond 180°

**中。** 在正 z 為前方的慣例下，θ 超過 90° 時 cosθ 為負，OCam 的 f(ρ) 因而可為負；這不是負的物體距離。OCam 的 cam2world 典型程式將多項式值放入第三分量再正規化，並沒有要求該值必須為正。實際係數的正負仍須依工具座標方向判讀。f=0 而 ρ>0 時，向量仍有效，正好表示赤道方向。[B4](#ref-b4)

**EN.** With positive z forward, cosθ becomes negative beyond θ=90°, so OCam’s f(ρ) can be negative. This is not a negative object distance. The typical cam2world implementation places the polynomial value in the third component and normalizes it, without requiring that value to be positive. Actual coefficient signs must still be interpreted using the tool’s axis convention. When f=0 and ρ>0, the vector remains valid and represents an equatorial direction. [B4](#ref-b4)

**中。** 您指出「過赤道後 sinθ 變小」是正確的，但它描述的是單位向量的橫向長度。等距影像半徑仍是 ρ=Fθ，可以持續增加。OCam 中 q 未固定長度；正規化後的橫向長度是 ρ/√(ρ²+f²)=sinθ。因此球面橫向分量縮小與影像半徑增大可以同時成立，並無矛盾。兩模型都透過光線方向與半徑的關係包含這個幾何現象。

**EN.** Your observation that sinθ decreases beyond the equator is correct, but it describes the transverse length of a unit vector. Equidistant image radius remains ρ=Fθ and can continue increasing. OCam’s q has no fixed length; its normalized transverse length is ρ/√(ρ²+f²)=sinθ. A shrinking spherical transverse component and an increasing image radius can therefore coexist. Both models incorporate this geometry through their ray-to-radius relationships.

**中。** KB 可在 θ>90° 使用，前提是模型與實作允許該角度，且在有效範圍內半徑映射保持單值。對徑向 KB，可檢查 P′(θ)>0；對 OCam，θ=atan2(ρ,f(ρ)) 的斜率如下。分母是正的向量長度平方，若分子 f−ρf′ 為正，θ 就隨 ρ 增加。這是可用的數學檢查，不是所有鏡頭係數自動滿足的保證。

**EN.** KB can operate at θ>90° if the model and implementation allow it and the radial mapping remains one-to-one over the valid domain. For radial KB, check P′(θ)>0. For OCam, the slope of θ=atan2(ρ,f(ρ)) is shown below. The denominator is a positive squared vector length; a positive numerator f−ρf′ makes θ increase with ρ. This is a useful mathematical check, not a property guaranteed for every coefficient set.

$$
\frac{d\theta}{d\rho}=\frac{f(\rho)-\rho f'(\rho)}{\rho^2+f(\rho)^2} \qquad\mathrm{(B9)}
$$

**中。** 校正必須實際覆蓋欲使用的後半球區域，並檢查反投影、再投影及有效影像遮罩。接近 θ=π 的正後方，方位角退化，不能單靠公式宣稱一張有限影像能無奇異地涵蓋整球。模型允許某角度、軟體接受該角度、硬體形成可用影像、以及實驗證明精度，是四個不同問題。

**EN.** Calibration must actually cover the rear-hemisphere region to be used, with checks on unprojection, reprojection, and the valid-image mask. At the directly backward direction θ=π, azimuth degenerates; the formulas alone do not establish a nonsingular representation of the entire sphere in one finite image. Model admissibility, software support, usable optical imaging, and experimentally demonstrated accuracy are separate questions.

## 9. 實施步驟：相同資料，不同參數化 / Implementation: shared data, different parameterizations

| 階段 / Stage | 共同工作 / Shared work | 主要差異 / Main difference |
|---|---|---|
| 採集 / Capture | 已知標靶、多姿態、覆蓋有效視野 / Known target, varied poses, valid-field coverage | 初始工具可能要求不同影像條件 / Initialization tools may impose different requirements |
| 初始化 / Initialize | 估計中心、尺度、標靶姿態 / Estimate center, scale, target poses | KB 與 OCam 使用各自代數結構 / Model-specific algebra |
| 擬合 / Fit | 以觀測與預測差異估參數 / Fit observation–prediction discrepancies | KB 擬合角度半徑關係；OCam 擬合軸向函數 / Angular-radius versus axial-function fit |
| 改善 / Refine | 聯合改善姿態、內參與誤差 / Refine poses, intrinsics, residuals | 自由參數與目標函數依實作 / Parameters and objective depend on implementation |
| 執行 / Evaluate | 像素與光線互換 / Pixel–ray conversion | KB 常對角度求逆；OCam 常建立反向近似 / Angular inversion versus inverse approximation |
| 驗證 / Validate | 留出資料、分角度報告、檢查三維任務 / Held-out data, angle bins, 3D task checks | 使用相同測試條件，不能僅比訓練誤差 / Match test conditions; training error is insufficient |

**中。** 原始 OCam 論文提出的初始化包含利用可見邊界與其幾何；後續工具或流程可能不同。比較時應列出使用的版本、參數數目、是否估仿射變形、殘差定義及停止條件。只說「都是棋盤校正」會忽略求解流程；只說「求解不同」又會掩蓋共同的幾何與資料基礎。[B1](#ref-b1)、[B2](#ref-b2)

**EN.** Initialization in the original OCam paper includes using the visible boundary and its geometry; later tools may differ. Comparisons should identify software versions, parameter counts, affine parameters, residual definitions, and stopping criteria. Saying only “both calibrate with a checkerboard” overlooks their solvers; saying only “their solvers differ” obscures their shared geometry and data. [B1](#ref-b1), [B2](#ref-b2)

## 10. 投射中心與直接三維度量 / Projection centers and direct 3D measurement

**中。** 標準 KB 與 OCam 的方向模型通常假設所有視線共用一個投射中心。這是模型假設，不是從多項式擬合良好就證明了實際鏡頭的所有光線精確交於一點。您的儀器若只量方向，可估計 n(u,v)；要研究匯聚區域大小，還需在共同儀器座標中量到每條光線的位置，例如每條光線上兩個不同距離的點，才能估計光線起點 o(u,v) 與方向。

**EN.** Standard KB and OCam direction models generally assume a shared projection center. A good polynomial fit does not prove that all physical rays intersect at exactly one point. If your instrument measures only direction, it can estimate n(u,v). Studying the convergence region additionally requires ray positions in a common instrument frame, for example two points at different distances on each ray, allowing estimation of both origin o(u,v) and direction.

$$
\mathbf X=\mathbf o(u,v)+\lambda\mathbf n(u,v) \qquad\mathrm{(B10)}
$$

**中。** 式 B10 允許不同像素有不同光線位置，是非中心描述。可找一個點使它到所有量測直線的垂直距離平方和最小，再報告 RMS、最大距離、角度分布及儀器不確定度；這些是描述中心近似程度的指標，不能全部稱作實體「匯聚球直徑」。非中心魚眼文獻確實探討視點隨方向改變的改善。[B5](#ref-b5)

**EN.** Equation B10 allows pixel-dependent ray locations, giving a noncentral description. One can fit a point minimizing the sum of squared perpendicular distances to measured lines, then report RMS, maximum distance, angular distribution, and instrument uncertainty. These quantify the quality of a central approximation; they are not all interchangeable with a physical “convergence-ball diameter.” Noncentral fisheye research explicitly studies improvements from direction-dependent viewpoints. [B5](#ref-b5)

**中。** 不必先轉成針孔影像就能度量：將兩台已校正相機的觀測轉成共同座標中的射線，再以三角測量求最符合兩條射線的三維點。已知基線或標準物提供公尺尺度；兩個三維點的距離就是物件尺寸的一種估計。角度誤差三角測量文獻直接以方向為輸入，但仍須滿足其中心相機、已知相對姿態等假設。[B6](#ref-b6)

**EN.** Measurement need not begin with a pinhole image: convert observations from two calibrated cameras to rays in a common frame, then triangulate the point best supported by those rays. A known baseline or reference object supplies metric scale; the distance between two reconstructed points estimates an object dimension. Angular-error triangulation works directly with directions, while retaining assumptions such as central cameras and known relative pose. [B6](#ref-b6)

## 11. 文獻證據能支持什麼？ / What do the published results establish?

**中。** KB 原始論文包含標稱 190° 魚眼實驗，另取影像的重投影 RMS 約為 0.13 像素（23 參數）及 0.16 像素（9 參數）；它們是影像預測殘差，不能直接改寫成物件尺寸的毫米精度。OCam 原始論文的反射折射相機三維實驗報告平均約 2.9 mm 的重建誤差；這不是所有折射式魚眼或後半球區域的通用精度保證。[B1](#ref-b1)、[B2](#ref-b2)

**EN.** The original KB paper includes a nominal 190° fisheye experiment, with approximately 0.13-pixel RMS for a 23-parameter model and 0.16 pixels for a 9-parameter model on an additional image. These are image-prediction residuals, not millimeter object-size accuracy. The original OCam paper reports approximately 2.9 mm mean reconstruction error in a catadioptric experiment; this is not a universal accuracy guarantee for refractive fisheyes or rear-hemisphere measurements. [B1](#ref-b1), [B2](#ref-b2)

**中。** Fu 等人的多魚眼校正草稿使用標稱 185° 鏡頭；其雙魚眼實驗在 20 個測試位置檢查 600 mm 標桿長度，報告 RMS 約 5.0890 mm。這是具有條件的長度測試，不是 KB 與 OCam 的直接優劣證據，也不是對所有大於 90° 偏軸角點的獨立報告。教學時應一併說明視場分布、量測範圍與校正及測試標準物的關係。[B7](#ref-b7)

**EN.** The multiple-fisheye calibration draft by Fu and colleagues uses nominal 185° lenses. Its two-fisheye experiment tests a 600 mm wand at 20 positions and reports about 5.0890 mm RMS length error. This conditional length test is neither a direct KB-versus-OCam ranking nor a separate evaluation of every point beyond θ=90°. Teaching should include angular coverage, measurement range, and the relationship between calibration and test reference objects. [B7](#ref-b7)

**中。** LF-VIO 研究使用環形全景視野，方位角 360°、偏軸角約 40°–120°，可用以討論後半球觀測對軌跡估計的影響。例如其中一組序列完整視野 ATE 約 0.093 m，前方子視野約 0.124 m。ATE 是軌跡誤差，不能當作單一後方物件點或尺寸的精度。要回答「後半球有多準」，應把驗證資料依 θ 分組，分別量方向誤差、位置誤差及尺寸誤差。[B8](#ref-b8)

**EN.** LF-VIO uses an annular panoramic field with 360° azimuth and roughly 40°–120° polar coverage, supporting discussion of rear-hemisphere observations in trajectory estimation. In one sequence, full-field ATE is about 0.093 m versus 0.124 m for a front-only subset. ATE is trajectory error, not the accuracy of an individual rear object point or dimension. To establish rear-hemisphere accuracy, validation should be grouped by θ and separately report directional, positional, and dimensional errors. [B8](#ref-b8)

## 12. 大語言模型能取代校正嗎？ / Can language models replace calibration?

**中。** 大語言模型可以協助推導、程式、文獻整理與校正品質檢查；但文字推理本身沒有提供您的實際鏡頭光線資料。學習式視覺模型則可能預測光線、相機參數或深度，改變參數化及估計流程。例如 UniK3D 使用學習式球面相機表示與三維預測；它不是單靠大語言模型閱讀文字就完成儀器校正。[B9](#ref-b9)

**EN.** Language models can assist with derivations, code, literature review, and calibration-quality checks, but textual reasoning does not supply the measured rays of your physical lens. Learned vision systems may predict rays, camera parameters, or depth, changing the representation and estimator. UniK3D, for example, uses a learned spherical camera representation and 3D prediction; this is not instrument calibration achieved merely by a language model reading text. [B9](#ref-b9)

**中。** 若要宣稱替代 KB 或 OCam，應在相同資料與獨立真值下比較視野覆蓋、角度誤差、尺寸誤差、跨距離穩定性及失敗案例。只以 KB 產生的標籤訓練，再對同一標籤評估，主要證明近似 KB 的能力，不能獨立證明物理正確性。您的逐像素儀器資料可作為評估方向，但本次沒有讀取博士論文或實驗資料，因此不對其新穎性或精度作結論。

**EN.** A replacement claim should compare field coverage, angular error, dimensional error, stability across distances, and failures using matched data and independent ground truth. Training and evaluating only against KB-generated labels mainly demonstrates approximation of KB, not independent physical correctness. Your per-pixel instrument measurements could support such evaluation; this session has not examined the thesis or experimental data, so no novelty or accuracy claim is made for them.

## 13. 課堂 Q&A / Classroom Q&A

### Q1. KB 是否沒有影像到物件空間映射？ / Does KB lack image-to-object-space mapping?

**中。** 有。解出 θ 再用式 B4 可得到方向。缺少的是單張像素的距離 λ，而這個限制也適用於 OCam。

**EN.** It has that mapping: solve for θ and use Equation B4 to obtain direction. A single pixel lacks range λ, a limitation shared by OCam.

### Q2. OCam 是否要做兩次物理校正？ / Does OCam require two physical calibrations?

**中。** 不必。常見第二個多項式由第一個已校正模型的取樣產生，以加速反向運算。應區分物理參數估計與數值近似。

**EN.** Not necessarily. The common second polynomial is fitted to samples of the calibrated first model to accelerate reverse evaluation. Physical parameter estimation and numerical approximation are distinct.

### Q3. 自變數是否一定是直接量到的量？ / Must an independent variable be directly observed?

**中。** 不一定。它表示函數輸入的位置；KB 的 θ 常由待估姿態間接取得，OCam 的 ρ 也依賴中心等參數。

**EN.** No. It identifies the function’s input role. KB’s θ often comes indirectly from an estimated pose, while OCam’s ρ depends on parameters such as the image center.

### Q4. 是否只是同一模型換基底？ / Is this merely a basis change within one model?

**中。** 一般不是。式 B6 包含反函數及 cot，有限次多項式經此轉換通常不再是有限次多項式。幾何可一致，有限參數函數族仍不同。

**EN.** Generally not. Equation B6 involves inversion and cotangent, which usually do not preserve finite polynomial form. The geometry can agree while the finite-parameter families differ.

### Q5. f(ρ)<0 是否無物理意義？ / Is f(ρ)<0 unphysical?

**中。** 在本文的正 z 向前慣例下，它表示後半球方向，並非負的量測距離。需檢查實作座標慣例及有效區域。

**EN.** Under our positive-z-forward convention, it represents a rear-hemisphere direction, not negative measured distance. Check the implementation’s convention and valid domain.

### Q6. θ>90° 時影像半徑必須變短嗎？ / Must image radius shrink beyond θ=90°?

**中。** 不必。縮短的是單位球方向的橫向分量 sinθ。等距影像的半徑 Fθ 仍增長；不可混用未正規化影像向量與單位方向。

**EN.** No. The shrinking quantity is the unit direction’s transverse component sinθ. Equidistant image radius Fθ still grows; unnormalized image vectors and unit directions must not be conflated.

### Q7. 小像素殘差是否證明共同投射中心與毫米精度？ / Does a small pixel residual prove centrality and millimeter accuracy?

**中。** 不會。它是特定資料上的影像擬合結果。中心性需量測光線位置，公制精度需獨立三維或長度真值與不確定度分析。

**EN.** No. It describes image fitting on specific data. Centrality requires ray-location measurements; metric accuracy requires independent 3D or length ground truth and uncertainty analysis.

### Q8. 怎樣公平比較兩種方法？ / How should the methods be compared fairly?

**中。** 共用原始影像、角點與保留測試資料，明列參數、有效視角、求解與運算時間。再比較分角度的光線誤差，以及相同三維任務的結果；勿只比較訓練重投影誤差。

**EN.** Share images, corners, and held-out tests; state parameters, valid angles, solver settings, and runtime. Compare angularly binned ray errors and the same 3D task, rather than only training reprojection error.

## 14. 延伸閱讀與課堂實作 / Further reading and classroom activities

**中。** 主題一：先讀 KB 的徑向模型與 backward model，再讀 Scaramuzza 的光線表示與校正步驟，最後讀 Double Sphere 第 2.4 節。活動：把各文獻的「輸入、輸出、未知係數、角度定義」整理成四欄表。檢核點：原始 OCam 的 f 與快速反向 g 不是同一個函數。[B1](#ref-b1)–[B4](#ref-b4)

**EN.** Topic 1: Read KB’s radial and backward models, then Scaramuzza’s ray representation and calibration procedure, and finally Double Sphere Section 2.4. Activity: tabulate each formulation’s input, output, unknown coefficients, and angle definition. Check: OCam’s original f and its fast reverse g are different functions. [B1](#ref-b1)–[B4](#ref-b4)

**中。** 主題二：以理想等距投射產生 θ=0°–110° 的合成資料，將 f(ρ)=ρcot(ρ/F) 擬合成不同次數的多項式。活動：用保留角度比較光線誤差，不只看 f 值誤差，並檢查式 B9 的單調性。這是模型近似練習，不是鏡頭實驗。

**EN.** Topic 2: Generate synthetic equidistant data over θ=0°–110° and fit f(ρ)=ρcot(ρ/F) with different polynomial degrees. Activity: compare ray errors at held-out angles, not only errors in f, and check monotonicity with Equation B9. This tests model approximation, not a physical lens.

**中。** 主題三：設計不經針孔重取樣的雙目度量實驗，閱讀角度三角測量及非中心模型文獻。活動：把真值、尺度來源、角度分組、距離分組及不確定度列成實驗表；分別回答「光線對嗎」「位置對嗎」「尺寸對嗎」。可搭配[教材 A 的取樣密度討論](Job-004_cdx-A-angular-sampling-teaching.md)，避免把像素數直接當作量測精度。[B5](#ref-b5)、[B6](#ref-b6)

**EN.** Topic 3: Design a stereo measurement experiment without pinhole resampling, informed by angular triangulation and noncentral models. Activity: specify ground truth, scale source, angle bins, distance bins, and uncertainty, separately asking whether rays, positions, and dimensions are correct. Pair this with [Handout A on sampling density](Job-004_cdx-A-angular-sampling-teaching.md) to avoid equating pixel count with measurement accuracy. [B5](#ref-b5), [B6](#ref-b6)

## 15. 參考文獻與使用範圍 / References and scope of use

<a id="ref-b1"></a>

[B1] Kannala, J., & Brandt, S. S. (2006). A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses. IEEE TPAMI, 28(8), 1335–1340. [Author PDF](https://users.aalto.fi/~kannalj1/calibration/Kannala_Brandt_calibration.pdf) · [DOI](https://doi.org/10.1109/TPAMI.2006.153)

**中。** 用途：原始徑向基底、反投影、非對稱項及 190° 實驗。本文的單純徑向比較未涵蓋所有原始參數。

**EN.** Used for the original radial basis, backward mapping, asymmetric terms, and 190° experiment. Our radial comparison does not cover every original parameter.

<a id="ref-b2"></a>

[B2] Scaramuzza, D., Martinelli, A., & Siegwart, R. (2006). A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. ICVS. [Author PDF](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf)

**中。** 用途：一般多項式光線表示、代數校正步驟及反射折射三維實驗；勿將該實驗自動外推至所有魚眼。

**EN.** Used for the general polynomial ray representation, algebraic calibration, and catadioptric 3D experiment; its results should not be generalized to every fisheye.

<a id="ref-b3"></a>

[B3] Usenko, V., Demmel, N., & Cremers, D. (2018). The Double Sphere Camera Model. [arXiv:1807.08957](https://arxiv.org/abs/1807.08957)

**中。** 用途：第 2.4 節對 Scaramuzza 與 KB 的比較；其中引文編號屬於該篇論文自己的文獻表。

**EN.** Used for the comparison of Scaramuzza and KB in Section 2.4; its citation numbers refer to that paper’s own bibliography.

<a id="ref-b4"></a>

[B4] OCamCalib source, NXP mirror of author-attributed code. [cam2world.m](https://github.com/nxp-imx/OCamCalib/blob/master/cam2world.m) · [world2cam_fast.m](https://github.com/nxp-imx/OCamCalib/blob/master/world2cam_fast.m) · [findinvpoly.m](https://github.com/nxp-imx/OCamCalib/blob/master/findinvpoly.m)

**中。** 用途：核對正規化、仰角及反向多項式建立流程；線上分支會變動，正式重現時應保存版本或 commit。

**EN.** Used to inspect normalization, elevation angle, and inverse-polynomial construction. Online branches can change; reproducible work should preserve a version or commit.

<a id="ref-b5"></a>

[B5] Tezaur, R., Kumar, A., & Nestares, O. (2022). A New Non-central Model for Fisheye Calibration. CVPR Workshops. [Open-access paper](https://openaccess.thecvf.com/content/CVPR2022W/OmniCV/papers/Tezaur_A_New_Non-Central_Model_for_Fisheye_Calibration_CVPRW_2022_paper.pdf)

**中。** 用途：方向相關視點與非中心校正；像素預測改善不等同於公制尺寸誤差。

**EN.** Used for direction-dependent viewpoints and noncentral calibration; improved pixel prediction is not equivalent to metric dimensional error.

<a id="ref-b6"></a>

[B6] Lee, S. H., & Civera, J. (2019). Closed-Form Optimal Two-View Triangulation Based on Angular Errors. ICCV. [Open-access paper](https://openaccess.thecvf.com/content_ICCV_2019/html/Lee_Closed-Form_Optimal_Two-View_Triangulation_Based_on_Angular_Errors_ICCV_2019_paper.html)

**中。** 用途：原生方向的三角測量；最適性依該文的角度誤差準則與假設，不是對所有像素誤差準則成立。

**EN.** Used for native-direction triangulation. Optimality depends on the paper’s angular-error criteria and assumptions, not every pixel-error objective.

<a id="ref-b7"></a>

[B7] Fu, Q., Quan, Q., & Cai, K.-Y. (2014). Calibration of Multiple Fish-Eye Cameras Using a Wand. [arXiv:1407.1267v1](https://arxiv.org/abs/1407.1267v1)

**中。** 用途：185° 鏡頭與標桿長度測試的草稿版本結果；保留其場景與標準物條件。

**EN.** Used for draft-version results with 185° lenses and wand-length tests, retaining their scene and reference-object conditions.

<a id="ref-b8"></a>

[B8] Wang, Z., et al. (2022). LF-VIO: A Visual-Inertial-Odometry Framework for Large Field-of-View Cameras with Negative Plane. [arXiv:2202.12613](https://arxiv.org/abs/2202.12613)

**中。** 用途：大視野與負成像平面觀測的軌跡實驗；第 11 節引用的是序列 ID06 的比較，不是所有序列平均。

**EN.** Used for trajectory experiments with large fields and negative-plane observations. Section 11 cites the ID06 comparison, not an average over all sequences.

<a id="ref-b9"></a>

[B9] Piccinelli, L., et al. (2025). UniK3D: Universal Camera Monocular 3D Estimation. CVPR. [arXiv:2503.16591](https://arxiv.org/abs/2503.16591)

**中。** 用途：學習式通用相機與三維估計的延伸閱讀；不作為使用者儀器或特定鏡頭精度的證據。

**EN.** Used as further reading on learned universal-camera 3D estimation, not as accuracy evidence for the user’s instrument or a particular lens.
