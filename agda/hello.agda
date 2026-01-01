{-# OPTIONS --exact-split #-}
import Relation.Binary.PropositionalEquality as Eq
open Eq using (_≡_; refl)
open Eq.≡-Reasoning using (begin_; step-≡-∣; _∎)

-- Natural numbers
data ℕ : Set where
  zero : ℕ
  suc  : ℕ → ℕ
{-# BUILTIN NATURAL ℕ #-}

-- Addition
_+_ : ℕ → ℕ → ℕ
zero + n = n
suc m + n = suc (m + n)

{-# BUILTIN NATPLUS _+_ #-}

_ : 2 + 3 ≡ 5
_ = refl

-- Multiplication
_*_ : ℕ → ℕ → ℕ
zero    * n = zero
(suc m) * n = n + (m * n)

_ : 2 * 3 ≡ 6
_ = refl

-- Power
_^_ : ℕ → ℕ → ℕ
m ^ zero    = suc (zero)
m ^ suc (n) = m * (m ^ n)

_ : 2 ^ 2 ≡ 4
_ = refl
_ : 4 ^ 2 ≡ 16
_ = refl
_ : 0 ^ 1 ≡ 0
_ = refl
_ : 1 ^ 1 ≡ 1
_ = refl
_ : 256 ^ 0 ≡ 1
_ = refl

-- Monus
_∸_ : ℕ → ℕ → ℕ
m     ∸ zero  = m
zero  ∸ suc n = zero
suc m ∸ suc n = m ∸ n

_ : 3 ∸ 2 ≡ 1
_ = refl
_ : 2 ∸ 3 ≡ 0
_ = refl
_ : 0 ∸ 2 ≡ 0
_ = refl
_ : 2 ∸ 0 ≡ 2
_ = refl

infixl 6 _+_ _∸_
infixl 7 _*_

_ : 3 * 2 + 4 ≡ 10
_ = refl
_ : 3 * 2 ∸ 1 ≡ 5
_ = refl
_ : 3 ∸ 2 * 2 ≡ 0
_ = refl

{-# BUILTIN NATTIMES _*_ #-}
{-# BUILTIN NATMINUS _∸_ #-}
