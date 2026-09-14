# ksf_FA_DynamicPricing - Test Plan Document

**Document Version:** 1.0  
**Date:** May 13, 2026  
**Module:** FA_DynamicPricing (FrontAccounting Dynamic Pricing Engine)  
**Status:** Approved

---

## 1. Introduction

### 1.1 Purpose

Comprehensive testing strategy for the FA_DynamicPricing module.

### 1.2 Test Categories

1. **Unit Testing** - Individual rule implementations
2. **Integration Testing** - FA integration, CRM integration
3. **System Testing** - End-to-end pricing workflows

---

## 2. Test Scenarios

### 2.1 Rule Creation Tests

**TC-DP-001: Create Bulk Discount Rule**
```
Test ID: TC-DP-001
Title: Create bulk discount with quantity condition
Priority: Critical

Test Steps:
1. Create rule with:
   - Name: "Buy 10+ Save 15%"
   - Type: Bulk Discount
   - Condition: min_quantity = 10
   - Action: 15% off
2. Save rule
3. Verify rule in database

Pass Criteria: Rule created with correct settings
```

**TC-DP-002: Create Category Discount**
```
Test ID: TC-DP-002
Title: Create category-based discount
Priority: High

Test Steps:
1. Create rule for Electronics category
2. Set 10% discount
3. Save and verify

Pass Criteria: Category rule created
```

**TC-DP-003: Create BOGO Rule**
```
Test ID: TC-DP-003
Title: Create buy 2 get 1 free rule
Priority: High

Test Steps:
1. Create BOGO rule
2. Set buy 2, get 1 at 100% off
3. Save

Pass Criteria: BOGO rule created correctly
```

---

### 2.2 Rule Evaluation Tests

**TC-DP-004: Evaluate Bulk Discount**
```
Test ID: TC-DP-004
Title: Bulk discount applies at threshold
Priority: Critical

Test Data:
- Rule: Buy 10+ get 15% off
- Cart: 10 units @ $100 each = $1000

Expected: $150 discount applied

Pass Criteria: Correct discount calculated
```

**TC-DP-005: Bulk Discount Below Threshold**
```
Test ID: TC-DP-005
Title: Bulk discount does not apply below quantity
Priority: Critical

Test Data:
- Rule: Buy 10+ get 15% off
- Cart: 5 units @ $100 each = $500

Expected: No discount applied

Pass Criteria: No discount for quantity < 10
```

**TC-DP-006: Evaluate Category Discount**
```
Test ID: TC-DP-006
Title: Category discount applies to matching items
Priority: High

Test Data:
- Rule: 10% off Electronics
- Cart: 3 Electronics items ($300) + 2 Furniture ($200)

Expected: $30 discount (10% of $300)

Pass Criteria: Only electronics discounted
```

**TC-DP-007: Role-Based Discount**
```
Test ID: TC-DP-007
Title: Role-based discount for wholesale customer
Priority: High

Test Data:
- Rule: 20% off for wholesale
- Customer role: wholesale
- Order: $500

Expected: $100 discount

Pass Criteria: Wholesale customer gets 20%
```

**TC-DP-008: BOGO Calculation**
```
Test ID: TC-DP-008
Title: Buy 2 Get 1 Free calculation
Priority: High

Test Data:
- Rule: Buy 2 Get 1 free
- Cart: 6 units @ $50 each = $300

Expected: 2 free items = $100 discount

Pass Criteria: Correct free item calculation
```

---

### 2.3 Conflict Resolution Tests

**TC-DP-009: Non-Stackable Rule Conflict**
```
Test ID: TC-DP-009
Title: Highest priority wins for non-stackable
Priority: Critical

Test Data:
- Rule A (Priority 10): 15% off category
- Rule B (Priority 20): 10% off all
- Item matches both, non-stackable

Expected: Rule A applies (15%)

Pass Criteria: Higher priority (lower number) wins
```

**TC-DP-010: Stackable Rules Combine**
```
Test ID: TC-DP-010
Title: Stackable rules apply together
Priority: High

Test Data:
- Rule A (stackable): 10% off
- Rule B (stackable): $5 off
- Order: $100

Expected: $15 total discount

Pass Criteria: Both discounts apply
```

**TC-DP-011: Max Stackable Limit**
```
Test ID: TC-DP-011
Title: Stackable limit enforced
Priority: Medium

Test Data:
- Max stackable: 3
- 4 stackable rules match

Expected: First 3 rules apply (based on priority)

Pass Criteria: Only 3 rules apply
```

---

### 2.4 Date Validity Tests

**TC-DP-012: Rule Within Valid Dates**
```
Test ID: TC-DP-012
Title: Rule applies within date range
Priority: High

Test Data:
- Rule valid: 2026-06-01 to 2026-08-31
- Today: 2026-07-15

Expected: Rule applies

Pass Criteria: Rule active during validity
```

**TC-DP-013: Rule Before Valid Dates**
```
Test ID: TC-DP-013
Title: Rule does not apply before start date
Priority: High

Test Data:
- Rule valid: 2026-06-01 to 2026-08-31
- Today: 2026-05-15

Expected: Rule does not apply

Pass Criteria: Rule inactive before start
```

**TC-DP-014: Rule After Valid Dates**
```
Test ID: TC-DP-014
Title: Rule does not apply after end date
Priority: High

Test Data:
- Rule valid: 2026-06-01 to 2026-08-31
- Today: 2026-09-15

Expected: Rule does not apply

Pass Criteria: Rule inactive after end
```

---

### 2.5 Integration Tests

**TC-DP-015: Sales Order Integration**
```
Test ID: TC-DP-015
Title: Dynamic pricing applied to sales order
Priority: Critical

Test Steps:
1. Create sales order
2. Add items qualifying for discount
3. Complete order
4. Verify discount line item added
5. Verify total adjusted

Pass Criteria: Order shows discounts
```

**TC-DP-016: Invoice Integration**
```
Test ID: TC-DP-016
Title: Invoice reflects dynamic pricing
Priority: High

Test Steps:
1. Create invoice from sales order
2. Verify discounts transferred
3. Verify discount amounts match

Pass Criteria: Invoice accurate
```

---

### 2.6 Performance Tests

**TC-DP-017: Rule Evaluation Performance**
```
Test ID: TC-DP-017
Title: Rule evaluation completes within 100ms
Priority: High

Test Data:
- 100 active rules
- Cart with 50 items

Expected: Evaluation < 100ms

Pass Criteria: Performance target met
```

**TC-DP-018: Cache Performance**
```
Test ID: TC-DP-018
Title: Cache improves performance
Priority: Medium

Test: First call vs subsequent calls

Expected: Subsequent calls 10x faster

Pass Criteria: Cache effective
```

---

## 3. Test Data

### 3.1 Products

| SKU | Name | Category | Price |
|-----|------|----------|-------|
| ELEC-001 | Laptop | Electronics | $1000 |
| ELEC-002 | Mouse | Electronics | $50 |
| FURN-001 | Desk | Furniture | $300 |
| FURN-002 | Chair | Furniture | $150 |

### 3.2 Rules

| Rule Name | Type | Condition | Action |
|-----------|------|-----------|--------|
| Bulk 10+ | Bulk | qty >= 10 | 15% off |
| Electronics 10% | Category | Electronics | 10% off |
| Wholesale 20% | Role | wholesale | 20% off |
| BOGO | BOGO | buy 2 | get 1 free |

---

## 4. Pass Criteria

| Test Type | Pass Rate |
|-----------|-----------|
| Unit Tests | 100% |
| Integration Tests | 100% |
| System Tests | 100% |