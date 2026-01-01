{-# OPTIONS --exact-split #-}
import Relation.Binary.PropositionalEquality as Eq
open Eq using (_≡_; refl; cong; sym)
open Eq.≡-Reasoning using (begin_; step-≡-∣; step-≡-⟩; _∎)
open import Data.Nat using (ℕ; zero; suc; _+_; _*_; _∸_; _^_)

--
-- BINARY
--

data Bin : Set where
  ⟨⟩ : Bin
  _O : Bin → Bin
  _I : Bin → Bin

inc : Bin → Bin
inc (⟨⟩)   = ⟨⟩ I
inc (b O)   = b I
inc (b I)   = (inc b) O

_ : inc(⟨⟩ O I) ≡ ⟨⟩ I O
_ = refl
_ : inc(⟨⟩ I I) ≡ ⟨⟩ I O O
_ = refl
_ : inc(⟨⟩ I O I I) ≡ ⟨⟩ I I O O
_ = refl

-- to(1) => ⟨⟩ I
-- to(suc(zero)) => ⟨⟩ I
-- to(2) => to(suc(suc(zero))) => ⟨⟩ I O

to : ℕ → Bin
to (zero) = ⟨⟩
to (suc(n)) = inc(to n)

_ : to(0) ≡ ⟨⟩
_ = refl
_ : to(1) ≡ ⟨⟩ I
_ = refl
_ : to(2) ≡ ⟨⟩ I O
_ = refl
_ : to(3) ≡ ⟨⟩ I I
_ = refl
_ : to(4) ≡ ⟨⟩ I O O
_ = refl

-- I O = 2
-- 2^1*1 + 2^0*0 = 2
--
--  2        1
-- from(I from(O))

from : Bin → ℕ
from (⟨⟩) = 0
from (b I) = suc(from(b) * 2)
from (b O) = from(b) * 2

_ : from (⟨⟩) ≡ 0
_ = refl
_ : from (⟨⟩ O) ≡ 0
_ = refl
_ : from (⟨⟩ I) ≡ 1
_ = refl
_ : from (⟨⟩ I O) ≡ 2
_ = refl
_ : from (⟨⟩ I I) ≡ 3
_ = refl
_ : from (⟨⟩ I O O) ≡ 4
_ = refl
_ : from (⟨⟩ I O I) ≡ 5
_ = refl
_ : from (⟨⟩ I I O) ≡ 6
_ = refl
_ : from (⟨⟩ I I I) ≡ 7
_ = refl

bin-frominc-eq-sucfrom : ∀ (a : Bin) → from (inc (a)) ≡ suc (from a)
bin-frominc-eq-sucfrom ⟨⟩ = refl
bin-frominc-eq-sucfrom (b O) = refl
bin-frominc-eq-sucfrom (b I) =
  begin
    from(inc (b I))
  ≡⟨⟩
    from(inc b O)
  ≡⟨⟩
    from(inc b) * 2
  ≡⟨ cong (_* 2) (bin-frominc-eq-sucfrom b) ⟩
    suc (from b) * 2
  ∎

-- to-from-bin : ∀ (b : Bin) → to (from b) ≡ b , does not hold as : (a : Bin ≡ ⟨⟩ O) ⟨⟩ O ≠ ⟨⟩

from-to-bin : ∀ (n : ℕ) → from (to (n)) ≡ n
from-to-bin zero = refl
from-to-bin (suc n) =
  begin
    from(to (suc n))
  ≡⟨⟩
    from(inc(to n))
  ≡⟨ bin-frominc-eq-sucfrom (to n) ⟩
    suc(from (to n))
  ≡⟨ cong suc (from-to-bin n) ⟩
    suc(n)
  ∎
