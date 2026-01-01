import Relation.Binary.PropositionalEquality as Eq
open Eq using (_≡_; refl; cong)
open import Data.Nat using (ℕ; zero; suc; _+_; _*_)
open import Data.Nat.Properties using (+-comm; +-identityʳ; *-comm)

data _≤_ : ℕ → ℕ → Set where
  z≤n : ∀ {n : ℕ}
    → zero ≤ n
  s≤s : ∀ {m n : ℕ}
    → m ≤ n
    → suc m ≤ suc n
  inv-s≤s : ∀ {m n : ℕ}
    → suc m ≤ suc n
    → m ≤ n

infix 4 _≤_

_ : 2 ≤ 4
_ = s≤s (s≤s z≤n)

