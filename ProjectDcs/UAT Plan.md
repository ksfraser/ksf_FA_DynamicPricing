# ksf_FA_DynamicPricing - UAT Plan Document

**Document Version:** 1.0  
**Date:** May 13, 2026  
**Module:** FA_DynamicPricing (FrontAccounting Dynamic Pricing Engine)  
**Status:** Pending UAT

---

## 1. Introduction

### 1.1 Purpose

User acceptance testing for the FA_DynamicPricing module.

### 1.2 UAT Objectives

1. Validate pricing rules work as expected
2. Verify discount calculations are accurate
3. Confirm integration with sales works correctly
4. Ensure reporting provides actionable data

---

## 2. UAT Scenarios

### 2.1 Marketing Scenarios

**UAT-DP-MKT-001: Create Summer Volume Discount**
```
Objective: Create summer bulk discount promotion

Test Steps:
1. Log in as Marketing Manager
2. Create bulk discount rule:
   - Name: "Summer Volume Discount"
   - Buy 25+ items get 20% off
   - Valid: June 1 - August 31, 2026
3. Verify rule active
4. Test with cart of 30 items
5. Confirm 20% discount applied
```

**Success Criteria:**
- [ ] Rule created successfully
- [ ] Discount calculates correctly
- [ ] Date range enforced

**Sign-off:** Marketing Manager _______________ Date: ________

---

**UAT-DP-MKT-002: Category Promotion**
```
Objective: Create category-wide promotion

Test Steps:
1. Create Electronics category discount
2. Set 15% off Electronics
3. Verify rule applies to electronics items
4. Confirm non-electronics unaffected
```

**Success Criteria:**
- [ ] Category rule works
- [ ] Only specified category affected

**Sign-off:** Marketing Manager _______________ Date: ________

---

**UAT-DP-MKT-003: BOGO Campaign**
```
Objective: Run buy 2 get 1 free promotion

Test Steps:
1. Create BOGO rule
2. Apply to specific category
3. Test with 6 items in cart
4. Verify 2 items free

Expected: 2 free items = significant discount
```

**Success Criteria:**
- [ ] BOGO calculates correctly
- [ ] Correct items marked free

**Sign-off:** Marketing Manager _______________ Date: ________

---

### 2.2 Sales Scenarios

**UAT-DP-Sales-001: Wholesale Pricing Automatic**
```
Objective: Verify wholesale customers get automatic discount

Test Steps:
1. Set up wholesale customer with role
2. Create sales order for wholesale customer
3. Add items to cart
4. Verify wholesale discount auto-applied
5. Compare to retail customer order
```

**Success Criteria:**
- [ ] Wholesale customer gets discount
- [ ] Retail customer sees standard price

**Sign-off:** Sales Manager _______________ Date: ________

---

**UAT-DP-Sales-002: Mixed Discounts**
```
Objective: Verify stackable discounts combine

Test Steps:
1. Set up stackable rule combination
2. Cart qualifies for multiple rules
3. Verify all discounts apply
4. Check total discount is correct
```

**Success Criteria:**
- [ ] Multiple rules stack
- [ ] Total is sum of all discounts

**Sign-off:** Sales Manager _______________ Date: ________

---

### 2.3 Finance Scenarios

**UAT-DP-FIN-001: Discount Reconciliation**
```
Objective: Verify discount amounts match invoices

Test Steps:
1. Review invoices from promotion period
2. Sum total dynamic discounts
3. Compare to invoice line items
4. Identify any discrepancies
```

**Success Criteria:**
- [ ] Discounts reconcile 100%
- [ ] No missing discounts

**Sign-off:** Finance Manager _______________ Date: ________

---

### 2.4 System Scenarios

**UAT-DP-SYS-001: Date Enforcement**
```
Objective: Verify rules activate/deactivate on dates

Test Steps:
1. Set rule with future start date
2. Test before start date - should not apply
3. Wait until start date
4. Test - should apply
5. Set rule with past end date
6. Test after end - should not apply
```

**Success Criteria:**
- [ ] Rules activate on start date
- [ ] Rules deactivate on end date

**Sign-off:** System Administrator _______________ Date: ________

---

**UAT-DP-SYS-002: Conflict Resolution**
```
Objective: Verify priority-based conflict resolution

Test Steps:
1. Create two conflicting rules
2. Set different priorities
3. Test cart qualifying for both
4. Verify highest priority wins

Expected: Only highest priority rule applies
```

**Success Criteria:**
- [ ] Conflicts resolved by priority
- [ ] Non-stackable rules don't combine

**Sign-off:** System Administrator _______________ Date: ________

---

## 3. Success Criteria

| Criteria | Threshold |
|----------|-----------|
| All scenarios executed | 100% |
| Critical defects resolved | 0 open |
| Business approval obtained | Yes |

---

## 4. Sign-off Section

| Role | Name | Signature | Date |
|------|------|-----------|------|
| Marketing Director | | | |
| Sales Director | | | |
| Finance Director | | | |
| IT Manager | | | |