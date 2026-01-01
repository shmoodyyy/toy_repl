import Relation.Binary.PropositionalEquality as Eq
open Eq using (_≡_; refl; cong; sym)
open Eq.≡-Reasoning using (begin_; step-≡-∣; step-≡-⟩; _∎)
open import Data.Nat using (ℕ; zero; suc; _+_; _*_; _∸_; _^_)

+-assoc : ∀ (m n p : ℕ) → (m + n) + p ≡ m + (n + p)
+-assoc zero n p =
  begin
    (zero + n) + p
  ≡⟨⟩
    n + p
  ≡⟨⟩
    zero + (n + p)
  ∎
+-assoc (suc m) n p =
  begin
    (suc m + n) + p
  ≡⟨⟩
    suc (m + n) + p
  ≡⟨⟩
    suc ((m + n) + p)
  ≡⟨ cong suc (+-assoc m n p) ⟩
    suc (m + (n + p))
  ≡⟨⟩
    suc m + (n + p)
  ∎

+-identity⃗ : ∀ (m : ℕ) → m + zero ≡ m
+-identity⃗ zero =
  begin
    zero + zero
  ≡⟨⟩
    zero
  ∎
+-identity⃗ (suc m) =
  begin
    suc m + zero
  ≡⟨⟩
    suc (m + zero)
  ≡⟨ cong suc (+-identity⃗ m) ⟩
    suc m
  ∎

+-suc : ∀ (m n : ℕ) → m + suc n ≡ suc (m + n)
+-suc zero n =
  begin
    zero + suc (n)
  ≡⟨⟩
    suc (n)
  ≡⟨⟩
    suc (zero + n)
  ∎
+-suc (suc m) n =
  begin
    suc m + suc n
  ≡⟨⟩
    suc (m + suc n)
  ≡⟨ cong suc (+-suc m n) ⟩
    suc (suc (m + n))
  ≡⟨⟩
    suc (suc m + n)
  ∎

+-comm : ∀ (m n : ℕ) → m + n ≡ n + m
+-comm m zero =
  begin
    m + zero
  ≡⟨ +-identity⃗ m ⟩
    m
  ≡⟨⟩
    zero + m
  ∎
+-comm m (suc n) =
  begin
    m + suc n
  ≡⟨ +-suc m n ⟩
    suc(m + n)
  ≡⟨ cong suc (+-comm m n) ⟩
    suc(n + m)
  ≡⟨⟩
    suc n + m
  ∎

+-rearrange : ∀ (m n p q : ℕ) → (m + n) + (p + q) ≡ m + (n + p) + q
+-rearrange m n p q =
  begin
    (m + n) + (p + q)
  ≡⟨ sym (+-assoc (m + n) p q) ⟩
    ((m + n) + p) + q
  ≡⟨ cong (_+ q) (+-assoc m n p) ⟩
    (m + (n + p)) + q
  ∎

+-swap : ∀ (m n p : ℕ) → m + (n + p) ≡ n + (m + p)
+-swap m n p =
  begin
    m + (n + p)
  ≡⟨ sym (+-assoc m n p) ⟩
    m + n + p
  ≡⟨ cong (_+ p) (+-comm m n) ⟩
    n + m + p
  ≡⟨ +-assoc n m p ⟩
    n + (m + p)
  ∎

*-zero : ∀ (m : ℕ) → m * zero ≡ zero
*-zero zero =
  begin
    zero * zero
  ≡⟨⟩
    zero
  ∎
*-zero (suc m) =
  begin
    suc m * zero
  ≡⟨⟩
    zero + (m * zero)
  ≡⟨ cong (zero +_) (*-zero m) ⟩
    zero + zero
  ≡⟨⟩
    zero
  ∎

*-suc : ∀ (m n : ℕ) → m * suc n ≡ m + m * n
*-suc zero n = refl
*-suc (suc m) n =
  begin
    suc m * suc n
  ≡⟨⟩
    suc n + m * suc n
  ≡⟨ cong (suc n +_) (*-suc m n) ⟩
    suc n + (m + m * n)
  ≡⟨ sym (+-assoc (suc n) m (m * n))⟩
    suc n + m + m * n
  ≡⟨ cong (_+ m * n) (+-comm (suc n) m) ⟩
    m + suc n + m * n
  ≡⟨ +-assoc m (suc n) (m * n) ⟩
    m + (suc n + m * n)
  ≡⟨⟩
    m + suc (n + m * n)
  ≡⟨ +-suc m (n + m * n) ⟩
    suc (m + (n + m * n))
  ∎

*-distrib-+ : ∀ (m n p : ℕ) → (m + n) * p ≡ m * p + n * p
*-distrib-+ m n zero rewrite *-zero (m + n) | *-zero m | *-zero n = refl
*-distrib-+ m n (suc p) =
  begin
    (m + n) * suc p
  ≡⟨ *-suc (m + n) p ⟩
    (m + n) + ((m + n) * p)
  ≡⟨ cong ((m + n) +_) (*-distrib-+ m n p) ⟩
    (m + n) + ((m * p) + (n * p))
  ≡⟨ sym (+-assoc (m + n) (m * p) (n * p)) ⟩
    (m + n) + (m * p) + (n * p)
  ≡⟨ cong (_+ n * p) (+-comm (m + n) (m * p)) ⟩
    (m * p) + (m + n) + (n * p)
  ≡⟨ cong (_+ n * p) (sym (+-assoc (m * p) m n)) ⟩
    (m * p + m) + n + (n * p)
  ≡⟨ +-assoc (m * p + m) n (n * p)⟩
    (m * p + m) + (n + n * p)
  ≡⟨ cong (_+ (n + n * p)) (+-comm (m * p) m) ⟩
    (m + m * p) + (n + n * p)
  ≡⟨ cong (m + m * p +_) (sym (*-suc n p)) ⟩
    m + m * p + n * suc p
  ≡⟨ cong (_+ n * suc p) (sym (*-suc m p)) ⟩
    m * suc p + n * suc p
  ∎

*-assoc : ∀ (m n p : ℕ) → (m * n) * p ≡ m * (n * p)
*-assoc zero n p  = refl
*-assoc (suc m) n p =
  begin
    (suc m * n) * p
  ≡⟨⟩
    (n + m * n) * p
  ≡⟨ *-distrib-+ n (m * n) p ⟩
    (n * p) + ((m * n) * p)
  ≡⟨ cong (n * p +_) (*-assoc m n p) ⟩
    (n * p) + (m * (n * p))
  ≡⟨⟩
    suc m * (n * p)
  ∎

*-comm : ∀ (m n : ℕ) → m * n ≡ n * m
*-comm m zero rewrite *-zero m = refl
*-comm m (suc n) =
  begin
    m * suc n
  ≡⟨ *-suc m n ⟩
    m + (m * n)
  ≡⟨ cong (m +_) (*-comm m n) ⟩
    m + n * m
  ≡⟨⟩
    suc n * m
  ∎ -- ≡ suc n * m

0∸≡0 : ∀ (n : ℕ) → zero ∸ n ≡ zero
0∸≡0 zero = refl
0∸≡0 (suc n) = refl

∸-+-assoc : ∀ (m n p : ℕ) → m ∸ n ∸ p ≡ m ∸ (n + p)
∸-+-assoc zero n p rewrite 0∸≡0 n | 0∸≡0 p | 0∸≡0 (n + p) = refl
∸-+-assoc (suc m) zero p = refl
∸-+-assoc (suc m) (suc n) p =
  begin
    suc m ∸ suc n ∸ p
  ≡⟨⟩
    m ∸ n ∸ p
  ≡⟨ ∸-+-assoc m n p ⟩
    m ∸ (n + p)
  ∎

^-distrib⃖-+-* : ∀ (m n p : ℕ) → m ^ (n + p) ≡ (m ^ n) * (m ^ p)
^-distrib⃖-+-* m zero zero = refl
^-distrib⃖-+-* m (suc n) zero =
  begin
    m ^ (suc n + zero)
  ≡⟨ cong (m ^_) (+-identity⃗ (suc n)) ⟩
    m ^ (suc n)
  ≡⟨⟩
    m * m ^ n
  ≡⟨ *-comm m (m ^ n)⟩
    m ^ (n) * m
  ≡⟨⟩
    m ^ (zero + n) * m
  ≡⟨ cong (_* m) (cong (m ^_) (+-comm zero n)) ⟩
    m ^ (n + zero) * m
  ≡⟨ *-comm (m ^ (n + zero)) m ⟩
    m * m ^ (n + zero)
  ≡⟨ cong (m *_) (^-distrib⃖-+-* m n zero) ⟩
    m * (m ^ n * m ^ zero)
  ≡⟨ sym (*-assoc m (m ^ n) (m ^ zero) ) ⟩
    m * m ^ n * m ^ zero
  ∎
^-distrib⃖-+-* m (suc n) (suc p) =
  begin
    m ^ (suc n + suc p)
  ≡⟨ cong (m ^_) ( +-comm (suc n) (suc p) ) ⟩
    m ^ (suc p + suc n)
  ≡⟨⟩
    m * m ^ (p + suc n)
  ≡⟨ cong (m *_) (cong (m ^_) ( +-comm p (suc n) ) ) ⟩
    m * m ^ (suc n + p)
  ≡⟨ cong (m *_) ( ^-distrib⃖-+-* m (suc n) p )⟩
    m * (m ^ suc n * m ^ p)
  ≡⟨ cong (m *_) (*-comm (m ^ suc n) (m ^ p)) ⟩
    m * (m ^ p * m ^ suc n)
  ≡⟨ sym (*-assoc m (m ^ p) (m ^ suc n)) ⟩
    m * m ^ p * m ^ suc n
  ≡⟨⟩
     m ^ suc p * m ^ suc n
  ≡⟨ *-comm (m ^ suc p) (m ^ suc n) ⟩
     m ^ suc n * m ^ suc p
  ∎
^-distrib⃖-+-* m zero (suc p) =
  begin
    m ^ (zero + suc p)
  ≡⟨⟩
    m ^ (suc (zero + p))
  ≡⟨⟩
    m * m ^ (zero + p)
  ≡⟨ cong (m *_) ( ^-distrib⃖-+-* m zero p ) ⟩
    m * (m ^ zero * m ^ p)
  ≡⟨ *-comm m (m ^ zero * m ^ p) ⟩
    (m ^ zero * m ^ p) * m
  ≡⟨ *-assoc (m ^ zero) (m ^ p) m ⟩
    m ^ zero * (m ^ p * m)
  ≡⟨ cong (m ^ zero *_) (*-comm (m ^ p) m) ⟩
    m ^ zero * (m * m ^ p)
  ∎
