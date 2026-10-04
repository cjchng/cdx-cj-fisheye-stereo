# 01 | KB and OCamCalib: mapping, inversion and field of view
# 01 | KB 與 OCamCalib：映射、求逆與視域

Research note / 研究札記 · 2026-09-29 · English + 繁體中文

## 1. Answer the three questions separately / 分別回答三個問題

**EN.** KB does describe image-to-object-space mapping: a pixel becomes a viewing ray, not a unique 3D point. A usual radial KB implementation calibrates one forward angular polynomial and obtains the backward mapping by inversion. OCamCalib calibrates a pixel-to-ray polynomial and commonly constructs a second polynomial for fast ray-to-pixel evaluation. This does not necessarily mean two independent physical calibrations. Numerical iteration is common for KB unprojection, but is not mathematically mandatory in every implementation. KB can represent directions beyond a 180° full field of view when its calibrated domain and implementation permit them.

**中。** KB 確實描述影像到物件空間的映射：像素會轉成一條視線，而非唯一的三維點。常見的徑向 KB 實作校正一個正向角度多項式，再透過求逆取得反向映射。OCamCalib 校正像素到光線的多項式，通常再建立第二個多項式，以快速計算光線到像素的映射；這不必然代表兩次獨立的實體校正。KB 反投影常使用數值迭代，但並非所有實作在數學上都必須如此。只要校正定義域與實作允許，KB 可以表示完整視野超過 180° 的方向。

## 2. Shared geometry and notation / 共用幾何與符號

**EN.** Use a camera coordinate system with forward optical axis +z. For X=(x,y,z), let r=sqrt(x²+y²), θ=atan2(r,z), and α=atan2(z,r). Away from the optical axis, θ is the polar angle from +z and α is signed elevation from the xy image plane; therefore α=π/2−θ. For a circular, rotationally symmetric field, full FoV=2θmax: 180°, 200° and 280° correspond to θmax=90°, 100° and 140°. A panoramic 360° azimuth is a different quantity. These equations use a declared convention; toolbox axis signs must be checked before comparing coefficients.

**中。** 採用光軸前方為 +z 的相機座標。對 X=(x,y,z)，定義 r=sqrt(x²+y²)、θ=atan2(r,z) 與 α=atan2(z,r)。在光軸以外，θ 是相對 +z 的極角，α 是相對 xy 影像平面的有號仰角，因此 α=π/2−θ。對旋轉對稱的圓形視域，完整 FoV=2θmax：180°、200° 與 280° 分別對應 θmax=90°、100° 與 140°。全景的 360° 方位角則是另一個量。這些公式採用明確約定；比較係數前必須檢查工具箱的座標軸正負方向。

## 3. KB forward and backward mappings / KB 正向與反向映射

**EN.** In the common KB4 radial form, d(θ)=θ+k1θ³+k2θ⁵+k3θ⁷+k4θ⁹, u=fx d(θ)x/r+cx, and v=fy d(θ)y/r+cy, with the optical-axis limit handled separately. Given a pixel, compute a=(u−cx)/fx, b=(v−cy)/fy and ρ=sqrt(a²+b²). Solve d(θ)=ρ in the calibrated interval, then obtain the unit bearing n=(sinθ a/ρ, sinθ b/ρ, cosθ). Thus the world ray is Xw=C+λRcw n, λ>0, where Rcw maps camera directions to world coordinates. Calibration provides n; stereo, motion, a known surface or another range constraint supplies λ. This derivation does not claim that a single image determines arbitrary depth.

**中。** 常見 KB4 徑向形式為 d(θ)=θ+k1θ³+k2θ⁵+k3θ⁷+k4θ⁹、u=fx d(θ)x/r+cx、v=fy d(θ)y/r+cy，光軸上的情況另以極限處理。給定像素，先計算 a=(u−cx)/fx、b=(v−cy)/fy 與 ρ=sqrt(a²+b²)，在已校正區間內解 d(θ)=ρ，再取得單位視線 n=(sinθ a/ρ, sinθ b/ρ, cosθ)。世界座標光線因此為 Xw=C+λRcw n、λ>0，其中 Rcw 將相機方向轉到世界座標。校正提供 n；立體視覺、移動觀測、已知表面或其他距離約束提供 λ。這項推導並不宣稱單張影像能決定任意深度。

**EN.** The original Kannala–Brandt paper also includes asymmetric terms and affine image transformation; “one polynomial” describes the usual radial core, not every parameter of every KB variant. Its section II-C explicitly presents a backward model. The original polynomial's leading coefficient and the modern focal-length-normalized KB4 convention should not be mixed when transferring coefficients. [KB06]

**中。** Kannala–Brandt 原始論文還包含非對稱項與影像仿射轉換；「一個多項式」描述的是常見徑向核心，而非所有 KB 變體的全部參數。原文第 II-C 節明確提出反向模型。原始多項式的首項係數與現代將尺度納入焦距的 KB4 約定不同，移植係數時不能混用。[KB06]

## 4. What OCamCalib's two polynomials mean / OCamCalib 的兩個多項式代表甚麼

**EN.** After removing center and affine terms, q=A⁻¹(p−c), ρ=||q||. The OCam-style backward model gives n proportional to (qx,qy,f(ρ)), with f(ρ)=a0+a1ρ+…+anρⁿ. Projection instead seeks ρ satisfying f(ρ)−(z/r)ρ=0, or uses an approximation ρ≈g(α). The author-attributed findinvpoly code samples angles, solves the polynomial equation, then fits g; it increases polynomial order toward a sampled maximum radius error below 0.01 pixel. That threshold measures approximation to the calibrated model, not physical camera accuracy. Both a physical calibration and a numerical approximation are involved, but a second independent target-acquisition session is not inherently required. [OC06, OCcode]

**中。** 移除中心與仿射項後，q=A⁻¹(p−c)、ρ=||q||。OCam 類型的反向模型使 n 與 (qx,qy,f(ρ)) 成比例，其中 f(ρ)=a0+a1ρ+…+anρⁿ。投影方向則求解 f(ρ)−(z/r)ρ=0，或使用近似式 ρ≈g(α)。標示原作者的 findinvpoly 程式會取樣角度、求多項式根，再擬合 g，並提高多項式階數，使取樣點的最大半徑誤差降到 0.01 像素以下。此門檻衡量對已校正模型的近似誤差，而不是實體相機精度。這裡包含實體校正與數值近似兩件事，但不必然需要第二次獨立拍攝校正標靶。[OC06, OCcode]

## 5. Must KB iterate? / KB 一定要迭代嗎？

**EN.** No. Newton's method is a frequent implementation choice: θnext=θ−(d(θ)−ρ)/d′(θ). A bracketed solver is safer where Newton steps could leave the valid interval. Alternatives include a precomputed lookup table with interpolation, a fitted inverse polynomial, or a numerical polynomial-root solver with physical-root selection. The first two move much of the work offline and introduce controllable approximation error; a root solver is not equivalent to a simple closed-form inverse. In the ideal equidistant special case d(θ)=θ, inversion is immediate. A general ninth-degree polynomial has no general radical formula. Calibration itself can still require nonlinear optimization even when runtime evaluation is non-iterative.

**中。** 不一定。牛頓法是常見實作：θnext=θ−(d(θ)−ρ)/d′(θ)。當牛頓步驟可能離開有效區間時，有界根搜尋通常較穩妥。替代方法包括預先建立查表並插值、擬合反向多項式，或用數值多項式求根器再選取物理解。前兩者將大部分工作移到離線階段，並引入可控制的近似誤差；數值求根器則不等於簡單的閉式反函數。在理想等距特例 d(θ)=θ 下，可直接求逆。一般九次多項式沒有通用根式公式。即使執行時不迭代，校正階段仍可能需要非線性最佳化。

## 6. Beyond 180°: possible, but domain-dependent / 超過 180°：可行，但取決於定義域

**EN.** At θ=90°, the ray has nz=0; at θ>90°, nz<0. The atan2/sin/cos KB formulation remains meaningful across that boundary. The original paper tested a 190° ORIFL190-3 lens: its additional-image reprojection RMS was 0.13 pixel for p23 and 0.16 pixel for p9 after estimating that image's pose. This establishes practical use above 180°, not millimetre object accuracy or a separately validated rear annulus. [KB06, section IV-B]

**中。** θ=90° 時光線 nz=0；θ>90° 時 nz<0。以 atan2、sin、cos 表達的 KB 公式可以跨越此邊界。原文測試了 190° ORIFL190-3 鏡頭：在額外影像上估計該影像姿態後，p23 與 p9 的重投影 RMS 分別為 0.13 與 0.16 像素。這證明了超過 180° 的實際使用，卻不是物件毫米精度，也不是對後半球環帶的獨立驗證。[KB06，第 IV-B 節]

**EN.** Require a unique physical inverse on the used interval, normally d′(θ)>0, and reject pixels outside the calibrated mask. A small d′ amplifies radial error because locally δθ≈δρ/d′(θ). An implementation that first divides by z, clamps θ to 90°, or rejects all negative-z points may impose a narrower domain than the model. Neither model automatically guarantees a valid 280° calibration; rearward samples, numerical tests and an occlusion/visibility mask are necessary. At the exact backward axis θ=π, the usual circular mapping also has a directional singularity.

**中。** 使用區間必須具有唯一的物理反函數，通常要求 d′(θ)>0，並排除校正遮罩以外的像素。d′ 很小會放大半徑誤差，因為局部關係為 δθ≈δρ/d′(θ)。若實作先除以 z、將 θ 限制在 90°，或排除所有負 z 點，實際可用範圍便可能小於模型本身。兩種模型都不會自動保證 280° 校正有效；必須有後向取樣、數值測試及遮蔽／可見性遮罩。在正後方光軸 θ=π，通常的圓形映射也有方向奇異性。

## Sources / 文獻與程式來源

[KB06] Kannala & Brandt (2006), A Generic Camera Model and Calibration Method for Conventional, Wide-Angle, and Fish-Eye Lenses. TPAMI. Sections II-C, IV-B. [Author manuscript / 作者稿](https://users.aalto.fi/~kannalj1/calibration/Kannala_Brandt_calibration.pdf). 中文參考譯名：一般、廣角與魚眼鏡頭的通用相機模型及校正方法。

[OC06] Scaramuzza, Martinelli & Siegwart (2006), A Flexible Technique for Accurate Omnidirectional Camera Calibration and Structure from Motion. ICVS. [Paper / 論文](https://rpg.ifi.uzh.ch/docs/ICVS06_scaramuzza.pdf). 中文參考譯名：精確全向相機校正與運動恢復結構的彈性方法。

[OCcode] Scaramuzza's OCamCalib author-attributed code, mirrored by NXP: [findinvpoly.m](https://github.com/nxp-imx/OCamCalib/blob/master/findinvpoly.m), [world2cam_fast.m](https://github.com/nxp-imx/OCamCalib/blob/master/world2cam_fast.m). 中文說明：擬合反向多項式與快速投影的原作者標示程式；此處連結為鏡像倉庫。

[DS18] Usenko, Demmel & Cremers (2018), The Double Sphere Camera Model, section 2.4. [Paper / 論文](https://arxiv.org/abs/1807.08957). 中文參考譯名：雙球相機模型。Its reference [12] is OC06, not the proposal's reference [12]. / 該文 [12] 為 OC06，並非研究計畫中的 [12]。
