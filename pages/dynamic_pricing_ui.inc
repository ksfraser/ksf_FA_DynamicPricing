<?php
/**
 * ksf_FA_DynamicPricing - Admin UI Pages
 */

/**
 * Main dynamic pricing admin page
 */
function dynamic_pricing_main() {
    page(_($help_context = "Dynamic Pricing Rules"));
    
    start_form();
    
    display_pricing_rules_table();
    
    submit_center('add_rule', _('Add New Rule'));
    
    end_form();
}

/**
 * Display all pricing rules in a table
 */
function display_pricing_rules_table() {
    $sql = "SELECT * FROM fa_pricing_rules WHERE 1=1 ORDER BY priority ASC, created_at DESC";
    $result = db_query($sql, "Could not get pricing rules");
    
    start_table(TABLESTYLE);
    $th = [_('Priority'), _('Rule Type'), _('Title'), _('Status'), _('Actions')];
    table_header($th);
    
    $k = 0;
    while ($rule = db_fetch($result)) {
        alt_table_row($k);
        
        label_cell($rule['priority']);
        label_cell(ucfirst($rule['rule_type']));
        label_cell($rule['title']);
        label_cell($rule['enabled'] ? _('Enabled') : _('Disabled'));
        
        $actions = "";
        $actions .= pager_link(_('Edit'), "dynamic_pricing_edit.php?rule_id=" . $rule['id']);
        $actions .= " | ";
        $actions .= pager_link(_('Delete'), "dynamic_pricing_delete.php?rule_id=" . $rule['id'], false, _('Confirm delete?'));
        
        label_cell($actions);
        
        end_row();
    }
    
    end_table();
}

/**
 * Add/Edit pricing rule page
 */
function dynamic_pricing_edit($rule_id = 0) {
    page(_($help_context = $rule_id ? "Edit Pricing Rule" : "Add Pricing Rule"));
    
    if (isset($_POST['save_rule'])) {
        $data = [
            'rule_type' => get_post('rule_type', 'percentage'),
            'title' => get_post('title', ''),
            'priority' => get_post('priority', 10),
            'stop_processing' => get_post('stop_processing', 0),
            'conditions' => json_encode([
                'min_quantity' => get_post('min_quantity', 0),
                'max_quantity' => get_post('max_quantity', 0),
                'category_ids' => get_post('category_ids', []),
                'user_roles' => get_post('user_roles', []),
                'min_cart_total' => get_post('min_cart_total', 0)
            ]),
            'config' => json_encode([
                'percentage' => get_post('percentage', 0),
                'amount' => get_post('amount', 0),
                'tiers' => get_post('tiers', [])
            ]),
            'enabled' => get_post('enabled', 1)
        ];
        
        if ($rule_id) {
            $sql = "UPDATE fa_pricing_rules SET 
                    rule_type=" . db_escape($data['rule_type']) . ",
                    title=" . db_escape($data['title']) . ",
                    priority=" . (int)$data['priority'] . ",
                    stop_processing=" . (int)$data['stop_processing'] . ",
                    conditions=" . db_escape($data['conditions']) . ",
                    config=" . db_escape($data['config']) . ",
                    enabled=" . (int)$data['enabled'] . "
                    WHERE id=" . (int)$rule_id;
            db_query($sql, "Could not update pricing rule");
            display_notification(_('Pricing rule updated successfully'));
        } else {
            $sql = "INSERT INTO fa_pricing_rules 
                    (rule_type, title, priority, stop_processing, conditions, config, enabled, created_at)
                    VALUES (
                    " . db_escape($data['rule_type']) . ",
                    " . db_escape($data['title']) . ",
                    " . (int)$data['priority'] . ",
                    " . (int)$data['stop_processing'] . ",
                    " . db_escape($data['conditions']) . ",
                    " . db_escape($data['config']) . ",
                    " . (int)$data['enabled'] . ",
                    NOW()
                    )";
            db_query($sql, "Could not insert pricing rule");
            display_notification(_('Pricing rule created successfully'));
        }
    }
    
    // Get existing rule if editing
    $rule = [];
    if ($rule_id) {
        $sql = "SELECT * FROM fa_pricing_rules WHERE id=" . (int)$rule_id;
        $result = db_query($sql, "Could not get pricing rule");
        $rule = db_fetch($result);
        $rule['conditions'] = json_decode($rule['conditions'], true);
        $rule['config'] = json_decode($rule['config'], true);
    }
    
    start_form();
    start_table(TABLESTYLE);
    
    text_row_ex(_('Title'), 'title', 50, 100, $rule['title'] ?? '');
    
    $rule_types = ['percentage' => _('Percentage Discount'), 'fixed' => _('Fixed Amount Discount'), 'bulk' => _('Bulk/Quantity Discount')];
    combo_list_row(_('Rule Type'), 'rule_type', $rule_types, $rule['rule_type'] ?? 'percentage');
    
    text_row_ex(_('Priority (lower = higher priority)'), 'priority', 10, 10, $rule['priority'] ?? 10);
    
    yesno_list_row(_('Stop Processing (apply this rule only)'), 'stop_processing', $rule['stop_processing'] ?? 0);
    yesno_list_row(_('Enabled'), 'enabled', $rule['enabled'] ?? 1);
    
    table_section_title(_('Conditions'));
    
    text_row_ex(_('Min Quantity'), 'min_quantity', 10, 10, $rule['conditions']['min_quantity'] ?? 0);
    text_row_ex(_('Max Quantity'), 'max_quantity', 10, 10, $rule['conditions']['max_quantity'] ?? 0);
    text_row_ex(_('Min Cart Total'), 'min_cart_total', 15, 20, $rule['conditions']['min_cart_total'] ?? 0);
    
    table_section_title(_('Configuration'));
    
    if (($rule['rule_type'] ?? 'percentage') === 'percentage') {
        text_row_ex(_('Discount Percentage'), 'percentage', 10, 10, $rule['config']['percentage'] ?? 0);
    } else {
        text_row_ex(_('Discount Amount'), 'amount', 15, 20, $rule['config']['amount'] ?? 0);
    }
    
    end_table();
    
    submit_center('save_rule', $rule_id ? _('Update Rule') : _('Create Rule'));
    end_form();
}

/**
 * Delete pricing rule
 */
function dynamic_pricing_delete($rule_id) {
    if (isset($_POST['confirm_delete'])) {
        $sql = "DELETE FROM fa_pricing_rules WHERE id=" . (int)$rule_id;
        db_query($sql, "Could not delete pricing rule");
        display_notification(_('Pricing rule deleted successfully'));
        meta_forward('dynamic_pricing_main.php');
    }
    
    page(_($help_context = "Delete Pricing Rule"));
    
    $sql = "SELECT title FROM fa_pricing_rules WHERE id=" . (int)$rule_id;
    $result = db_query($sql, "Could not get pricing rule");
    $rule = db_fetch($result);
    
    echo "<h3>" . _('Confirm Delete') . "</h3>";
    echo "<p>" . sprintf(_('Are you sure you want to delete the rule "%s"?'), $rule['title']) . "</p>";
    
    start_form();
    submit_center('confirm_delete', _('Delete'));
    end_form();
}
