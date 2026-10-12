# Simple Combat Arena: Polymorphism in Practice

> **Type:** C++ implementation assignment journal\
> **Status:** Core assignment completed\
> **Language:** Modern C++ (C++20-compatible)\
> **Scope:** Single source file; no dynamic allocation, containers, or
> smart pointers yet

This note records the design, debugging journey, and lessons from
building a small text-based combat simulation. It complements
\[\[Cpp-Polymorphism-and-OOP-Design\]\] rather than replacing it.

------------------------------------------------------------------------

## 1. Assignment overview

The goal was to combine object-oriented concepts into a small working
program.

**Central question:** How can one function work with different kinds of
objects through a shared interface, without checking each object's
concrete type?

The assignment used: - **Goblin** --- an enemy that can attack and take
damage. - **Dragon** --- another enemy with different attack behavior
and starting health. - **Dummy** --- something that can take damage but
is not an enemy and cannot attack.

### Intended class structure

``` mermaid
classDiagram
    class Damageable {
        <<abstract>>
        +takeDamage(int damage)*
        +virtual destructor
    }
    class Enemy {
        <<abstract>>
        -const char* name
        -int health
        +EnemyStatus()
        +takeDamage(int damage)
        +attack()*
    }
    class Goblin {
        +attack()
    }
    class Dragon {
        +attack()
    }
    class Dummy {
        +takeDamage(int damage)
    }
    Damageable <|-- Enemy
    Damageable <|-- Dummy
    Enemy <|-- Goblin
    Enemy <|-- Dragon
```

The key distinction is that `Enemy` is a `Damageable`, while `Dummy` is
also a `Damageable` without being an `Enemy`.

------------------------------------------------------------------------

## 2. The design I built

### `Damageable`: a capability interface

`Damageable` declares a pure virtual `takeDamage(int)` function and has
a virtual destructor.

``` cpp
class Damageable {
public:
    virtual void takeDamage(int damage) = 0;
    virtual ~Damageable() = default;
};
```

This interface says what an object can do, not what kind of object it
is.

### `Enemy`: shared state and behavior

`Enemy` derives from `Damageable`. It owns the shared enemy state---name
and health---and provides common damage handling. It also declares
`attack()` as pure virtual, so `Enemy` remains abstract.

``` cpp
class Enemy : public Damageable {
private:
    const char* name;
    int health;

public:
    virtual void attack() = 0;
};
```

This snippet illustrates the relationship, not the full implementation.
The actual class also contains constructors, status output, and damage
handling.

### Concrete enemies: base-constructor initialization

One stumbling block was figuring out how `Goblin` and `Dragon` should
initialize state held by `Enemy`.

``` cpp
Derived()
    : Base(/* arguments for the base constructor */)
{
}
```

`Goblin` and `Dragon` use this pattern to provide their names and
starting health. The base constructor initializes the shared members;
the derived class chooses the values. This avoids duplicating
initialization logic.

### `Dummy`: same capability, different identity

`Dummy` derives directly from `Damageable`, not `Enemy`. It can respond
to damage but has no attack behavior.

I deliberately chose **not** to give the dummy health. I considered a
wear-and-tear value instead, but did not add it to the assignment. That
could be a future feature if accumulated wear affects the dummy's
condition or usefulness.

------------------------------------------------------------------------

## 3. Runtime polymorphism in practice

The assignment used references to access concrete objects through a
base-class interface:

``` cpp
Goblin goblin;
Enemy& enemy = goblin;
```

The reference's static type is `Enemy&`, but the object it refers to is
a `Goblin`.

``` cpp
void fight(Enemy& enemy) {
    enemy.attack();
}
```

When passed a goblin or dragon, the virtual call invokes the concrete
object's `attack()` override.

### The generic damage function

The important abstraction was a function that accepts `Damageable&`
rather than `Enemy&`:

``` cpp
void applyDamage(Damageable& target, int damage) {
    target.takeDamage(damage);
}
```

This is a representative version of the design goal. The function
doesn't need to know whether `target` refers to a goblin, dragon, or
dummy. Virtual dispatch selects the matching override.

``` mermaid
flowchart TD
    A["applyDamage(target, amount)"] --> B["target is a Damageable reference"]
    B --> C{"Virtual dispatch"}
    C --> D["Enemy::takeDamage"]
    C --> E["Dummy::takeDamage"]
    D --> F["Enemy health behavior"]
    E --> G["Dummy-specific behavior"]
```

**Lesson:** A good interface lets the caller express what it needs
without depending on the concrete implementation.

------------------------------------------------------------------------

## 4. The debugging journey

This assignment was useful partly because the early versions were
imperfect. The following are the main lessons from the iterations.

### 4.1 Uninitialized state

**Problem:** I initially declared `name` and `health` without ensuring
both were initialized for every construction path.

**Why it mattered:** Reading an uninitialized pointer such as `name` can
produce undefined behavior. Initializing only `health` does not make
`name` safe.

**Lesson:** Every constructor must leave the object in a valid state.
Member initializer lists are a direct way to initialize members.

### 4.2 A base constructor was available, but derived classes weren't using it

**Problem:** `Enemy` had a constructor that accepted a name and starting
health, but the derived classes initially used the default constructor.

**Discovery:** `Goblin() : Enemy() {}` is valid, but it calls the
default `Enemy` constructor. To choose goblin-specific values, the
derived constructor must pass those values to the appropriate base
constructor.

**Lesson:** The base class manages shared state; the derived class can
choose the values that configure its base subobject.

### 4.3 The interface signatures did not match

**Problem:** The first `Damageable` declaration had `takeDamage()` with
no parameter, while implementations took an integer.

**Why it mattered:** `takeDamage()` and `takeDamage(int)` are different
function signatures. The latter does not override the former.

**Fix:** The interface and implementations were changed to use the same
signature, `takeDamage(int)`. Adding `override` lets the compiler check
the intended relationship.

**Lesson:** With virtual functions, the interface contract must match.
`override` is a valuable compiler-checked safeguard.

### 4.4 Printing a message is not the same as changing state

**Problem:** An early version of `takeDamage()` only printed a message
and did not reduce health.

**Lesson:** A state-changing operation must actually update the model.
Output can report the change, but it does not perform the change by
itself.

### 4.5 Invalid input must stop before state changes

**Problem:** The negative-damage check initially printed an error but
then allowed execution to continue. Subtracting a negative number
increases health.

**Fix:** Reject invalid damage and return immediately.

``` cpp
if (damage < 0) {
    std::cout << "Invalid damage points\n";
    return;
}
```

**Lesson:** Validate inputs before performing operations that depend on
them. An early return can make this control flow clear.

### 4.6 Condition order changed the meaning of damage

This was the biggest debugging lesson.

An early implementation checked whether damage exceeded health, changed
health to zero, and then checked whether health was zero. That could
mistake a killing blow for a hit against an enemy that had already been
defeated.

The logic became easier to reason about when divided into stages:

1.  Reject invalid damage.
2.  Check whether the enemy was already defeated.
3.  Apply damage to a living enemy.
4.  Ensure health does not go below zero.
5.  Report the outcome.

The important distinction is between: - **Already defeated before this
hit**, and - **Defeated by this hit**.

The final health value can be identical in both cases, but the events
and messages should differ.

### 4.7 Avoid redundant branches

After restructuring the damage logic, a separate `health < 0` branch was
no longer needed: the code set health to zero when damage exceeded
remaining health, and otherwise subtracted an amount no greater than
health.

**Lesson:** After changing logic, revisit older conditions. Some may
have become unreachable or redundant.

### 4.8 Test boundary cases, not just ordinary cases

    Starting health   Damage   Expected health What it checks
  ----------------- -------- ----------------- -------------------------
                300       50               250 Ordinary damage
                300      300                 0 Exact lethal damage
                300      350                 0 Damage exceeding health
                  0       10                 0 Hit after defeat
                100      -20               100 Invalid negative damage
                100        0               100 Zero damage

The table describes expected behavior, not a claim that every case was
separately verified during the assignment.

------------------------------------------------------------------------

## 5. Concepts I practised

  -----------------------------------------------------------------------
  Concept                             How it appeared in the assignment
  ----------------------------------- -----------------------------------
  Abstract class                      `Damageable` and `Enemy` cannot be
                                      instantiated directly

  Pure virtual function               Required derived classes to
                                      implement their capability or
                                      attack behavior

  Inheritance                         `Goblin` and `Dragon` are enemies;
                                      `Enemy` and `Dummy` are damageable

  Runtime polymorphism                Virtual calls through `Enemy&` and
                                      `Damageable&` select derived
                                      behavior

  Constructor initializer list        Concrete enemy constructors
                                      initialize the `Enemy` base
                                      subobject

  Encapsulation                       `name` and `health` were made
                                      private

  State invariants                    Health should not become negative;
                                      invalid damage should not modify
                                      state

  Virtual destructor                  Supports safe destruction through a
                                      base pointer when deletion through
                                      that interface is used

  Interface-based design              `applyDamage(Damageable&, int)`
                                      does not need concrete-type checks

  Debugging control flow              Condition order, early returns, and
                                      redundant branches changed program
                                      behavior
  -----------------------------------------------------------------------

------------------------------------------------------------------------

## 6. What I learned about design

### Design around capabilities

The dummy helped clarify why not every damageable object should inherit
from `Enemy`.

An inheritance relationship should communicate a genuine **is-a**
relationship. A dummy can receive damage, but that does not make it an
enemy.

### Separate the operation from the concrete type

The generic damage function expresses the operation the caller wants to
perform. It does not need to know the target's class.

This keeps the function open to additional `Damageable` implementations
without adding a type-checking branch for each one.

### Make state transitions explicit

Health is not merely an integer. It has rules:

-   Negative damage is invalid.
-   Health should not become negative.
-   An already-defeated enemy should not be treated as newly defeated.
-   A lethal hit should be distinguishable from a later hit.

Writing those rules down makes the implementation easier to reason about
and test.

### Keep the scope small

The assignment intentionally stopped before `std::vector`,
`std::unique_ptr`, dynamic allocation, and multi-file organization.
First, I needed to get the object model and interface behavior working
with local objects and references.

That is a deliberate learning sequence, not a missing requirement.

------------------------------------------------------------------------

## 7. Limitations and possible future improvements

These are ideas for later, not requirements to add retroactively:

-   **Dummy wear and tear:** Track accumulated use if it affects the
    dummy's condition or behavior.
-   **Safer name ownership:** `const char*` is adequate for pointing at
    string literals in this exercise, but `std::string` is more flexible
    when objects should own their names.
-   **Read-only accessors:** A `health()` or `name()` accessor could let
    other code inspect state without exposing writable fields. The
    current status-printing method is enough for this exercise.
-   **Zero-damage policy:** Decide whether zero damage should be
    reported as a hit or treated as a no-op.
-   **Damage and health types:** Consider overflow and other numeric
    edge cases if the simulation becomes more realistic.
-   **Collections and ownership:** Later, use a collection of smart
    pointers to manage a group of polymorphic enemies. This introduces
    ownership and lifetime questions that were intentionally out of
    scope here.

Avoid adding complexity just because it is possible. A feature is worth
implementing when it creates meaningful behavior or teaches the next
intended concept.

------------------------------------------------------------------------

## 8. Self-review questions

Try answering these without looking back at the code first.

1.  Why is `Enemy` abstract even though it implements `takeDamage()`?
2.  Why can `applyDamage(Damageable&, int)` work with both an enemy and
    a dummy?
3.  Why doesn't `Goblin` need to duplicate the initialization of `name`
    and `health`?
4.  Why is `takeDamage(int)` not an override of `takeDamage()`?
5.  Why must invalid damage be rejected before health is changed?
6.  Why does checking `health == 0` before applying a new hit change the
    program's behavior?
7.  Why is `Dummy` a `Damageable` but not an `Enemy`?
8.  What is the difference between the static type of an `Enemy&`
    expression and the dynamic type of the object it refers to?
9.  Why is a virtual destructor important in a polymorphic base class
    when an object may be deleted through a base pointer?
10. What new design problems appear when the arena stores many enemies
    dynamically?

### Short answers

1.  `Enemy::attack()` is still pure virtual.
2.  Both implement the `Damageable` contract; virtual dispatch chooses
    the override.
3.  The `Enemy` constructor owns the shared initialization logic.
4.  The parameter lists differ.
5.  Otherwise invalid input can modify state; negative damage could
    increase health.
6.  It distinguishes an enemy already dead before the hit from one
    defeated by the hit.
7.  The dummy can take damage but is not conceptually an enemy and
    cannot attack.
8.  The static type is the type known from the expression; the dynamic
    type is the actual object's type.
9.  It ensures derived destruction is handled correctly through the base
    interface.
10. Ownership, object lifetime, container design, and polymorphic
    storage need to be considered.

------------------------------------------------------------------------

## 9. Related notes

-   \[\[Cpp-Polymorphism-and-OOP-Design\]\] --- abstraction,
    encapsulation, virtual functions, slicing, capability interfaces,
    and virtual destructors.
-   \[\[Cpp-Inheritance-Foundations\]\] --- inheritance, access
    control, and base/derived construction and destruction.
-   \[\[Cpp-Constructors-and-Destructors\]\] ---
    constructor initialization and object lifetime.

------------------------------------------------------------------------

## Final takeaway

The most valuable outcome was not the combat output. It was learning to
combine concepts into a small, coherent program---and to debug the
*reasoning* behind a bug instead of only patching the symptom.

The next natural step is to extend the arena to manage multiple
polymorphic enemies, introducing `std::vector` and `std::unique_ptr`
when the fundamentals of this version feel solid.
