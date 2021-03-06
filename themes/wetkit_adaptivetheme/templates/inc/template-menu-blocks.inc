<?php

/**
 * @file
 * Process theme data.
 *
 * Use this file to run your theme specific implimentations of theme functions,
 * such preprocess, process, alters, and theme function overrides.
 *
 * Preprocess and process functions are used to modify or create variables for
 * templates and theme functions. They are a common theming tool in Drupal,
 * often used as an alternative to directly editing or adding code to templates.
 * Its worth spending some time to learn more about these functions - they are a
 * powerful way to easily modify the output of any template variable.
 *
 * Preprocess and Process Functions SEE:
 * http://drupal.org/node/254940#variables-processor
 * 1. Rename each function and instance of "wetkit_adaptivetheme" to match
 *    your subthemes name, for example, if your theme name is "footheme" then
 *    the function name will be "footheme_preprocess_hook". Tip - you can
 *    search/replace on "wetkit_adaptivetheme".
 * 2. Uncomment the required function to use.
 */

/**
 * Override or insert variables into the menu tree templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_tree__main_menu($variables) {
  return '<ul>' . $variables['tree'] . '</ul>';
}

/**
 * Override or insert variables into the menu tree templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_tree__menu_block($variables) {
  return $variables['tree'];
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_link__menu_block($variables) {
  $element = $variables['element'];
  $sub_menu = '';

  // Recursion.
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  // Depth 1.
  if ($element['#original_link']['depth'] == 1) {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    if (!empty($sub_menu)) {
      return '<section class="wet-menu-block"><h3 class="top-section">' . $output . '</h3>' . $sub_menu . '</section>';
    }
    else {
      return '<section class="wet-menu-block"><h3 class="top-section">' . $output . '</h3>' . '</section>';
    }
  }

  // Depth 2 with children.
  elseif ($element['#original_link']['depth'] == 2 && $element['#original_link']['has_children'] == 1) {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    //if (!empty($sub_menu) && in_array($element['#href'], array('<noitem>'))) {
    //  return '<div>' . '<ul>' . $sub_menu . '</ul>' . "</div>\n";
    //}
    if (!empty($sub_menu)) {
      return '<section><h4' . drupal_attributes($element['#attributes']) . '>' . $output . '</h4>' . '<ul>' . $sub_menu . '</ul>' . "</section>\n";
    }
    else {
      return '<section><h4' . drupal_attributes($element['#attributes']) . '>' . $output . '</h4>' . "</section>\n";
    }
  }

  // Depth 2 no children.
  elseif ($element['#original_link']['depth'] == 2 && $element['#original_link']['has_children'] == 0) {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    return '<div class="top-level">' . $output . '</div>';
  }

  // Depth 3 with children.
  elseif ($element['#original_link']['depth'] == 3 && $element['#original_link']['has_children'] == 1) {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    return '<li>' . $output . '<ul>' . $sub_menu . '</ul></li>';
  }

  // Depth 3 no children.
  elseif ($element['#original_link']['depth'] == 3 && $element['#original_link']['has_children'] == 0) {
    $output = '<li>' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</li>';
    return $output;
  }

  // Default Depth.
  else {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    return '<li>' . $output . "</li>\n";
  }
}

/**
 * Override or insert variables into the menu tree templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_tree__menu_block__wetkit_menu_blocks_1($variables) {
  return $variables['tree'];
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_link__menu_block__wetkit_menu_blocks_1($variables) {
  $element = $variables['element'];
  $sub_menu = '';
  static $tmp = '';

  // Logic for some mandatory menu attributes.
  if (isset($element['#localized_options']['attributes'])) {
    $tags = drupal_attributes($element['#localized_options']['attributes']);
  }
  else {
    $tags = '';
  }

  // Prepare the mega menu items that can't be handled through recursion.
  if ($element['#original_link']['depth'] == 1 && $element['#original_link']['has_children'] == 1) {
    $menu_items = array_intersect_key($element['#below'], array_flip(element_children($element['#below'])));
    foreach ($menu_items as $index => $value) {
      if ($value['#original_link']['has_children'] == 0) {
        $tmp .= '<li class="top-level">' . l($value['#title'], $value['#href'], $value['#localized_options']) . '</li>';
        unset($element['#below'][$index]);
      }
    }
    if (!empty($tmp)) {
      $tmp = '<ul>' . $tmp . '</ul>';
    }
  }

  // Recursion.
  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }

  // Depth 1 with children.
  if ($element['#original_link']['depth'] == 1 && $element['#original_link']['has_children'] == 1) {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = '<h3>' . $element['#title'] . '</h3>';
    }
    else {
      $output = '<h3>' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h3>';
    }
    if (!at_get_setting('render_mb_main_link')) {
      $mb_main_link = '<div class="mb-main-link">' . l($element['#title'] . ' - ' . t('Main Page'), $element['#href'], $element['#localized_options']) . '</div>';
      return '<li><section>' . $output . '<div class="mb-sm">' . $sub_menu . '<div class="clear"></div>' . $mb_main_link . '</div>' . "</section></li>\n";
    }
    else {
      return '<li><section>' . $output . '<div class="mb-sm">' . $sub_menu . '<div class="clear"></div></div>' . "</section></li>\n";
    }
  }
  
  // Depth 1 no children.
  elseif ($element['#original_link']['depth'] == 1 && $element['#original_link']['has_children'] == 0) {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = $element['#title'];
    }
    else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    }
    return '<li><div>' . $output . "</div></li>\n";
  }

  // Depth 2 with children.
  elseif ($element['#original_link']['depth'] == 2 && $element['#original_link']['has_children'] == 1) {
    if (in_array($element['#href'], array('<nolink>'))) {
      if (!at_get_setting('render_nolink')) {
        $output = '<h4 data-role="list-divider"' . $tags . '><a href="#">' . $element['#title'] . '</a></h4>';
      }
      else {
        $output = '<h4 data-role="list-divider"' . $tags . '>' . $element['#title'] . '</h4>';
      }
      return '<div class="span-2"><section>' . $output . '<ul>' . $sub_menu . '</ul>' . "</section></div>\n";
    }
    elseif (in_array($element['#href'], array('<noitem>'))) {
      return '<div class="span-2"><ul>' . $sub_menu . "</ul></div>\n";
    }
    else {
      $output = '<h4>' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h4>';
      $output = '<div class="span-2">' . $tmp . '<section>' . $output . '<ul>' . $sub_menu . '</ul>' . "</section></div>\n";
      $tmp = '';
      return $output;
    }
  }

  // Depth 2 no children.
  elseif ($element['#original_link']['depth'] == 2 && $element['#original_link']['has_children'] == 0) {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = $element['#title'];
    }
    else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    }
    return '<li class="top-level">' . $output . "</li>\n";
  }

  // Depth 3 with children.
  elseif ($element['#original_link']['depth'] == 3 && $element['#original_link']['has_children'] == 1) {
    $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    return '<li>' . $output . '<ul>' . $sub_menu . "</ul></li>\n";
  }

  // Default Depth.
  else {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = $element['#title'];
    }
    else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    }
    return '<li>' . $output . "</li>\n";
  }
}

/**
 * Override or insert variables into the menu tree templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_tree__menu_block__wetkit_menu_blocks_2($variables) {
  return $variables['tree'];
}

/**
 * Override or insert variables into the menu link templates.
 *
 * @param array $variables
 *   An array of variables to pass to the theme template.
 */
function wetkit_adaptivetheme_menu_link__menu_block__wetkit_menu_blocks_2($variables) {
  $element = $variables['element'];
  $sub_menu = '';

  // Custom Logic Based on Theme Selection (Internet, Intranet or Base)
  if (variable_get('wetkit_wetboew_theme', 0) == 2) {
    $theme_prefix = 'base';
  }
  else {
    $theme_prefix = 'gcwu';
  }

  if ($element['#below']) {
    $sub_menu = drupal_render($element['#below']);
  }
  if ($element['#original_link']['depth'] == 1) {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = '<h4 class="' . $theme_prefix . '-col-head">' . $element['#title'] . '</h4>';
    }
    else {
      $output = '<h4 class="' . $theme_prefix . '-col-head">' . l($element['#title'], $element['#href'], $element['#localized_options']) . '</h4>';
    }
    return '<section><div class="span-2">' . $output . '<ul>' . $sub_menu . '</ul>' . "</div></section>\n";
  }
  else {
    if (in_array($element['#href'], array('<nolink>'))) {
      $output = $element['#title'];
    }
    else {
      $output = l($element['#title'], $element['#href'], $element['#localized_options']);
    }
    return '<li>' . $output . $sub_menu . "</li>\n";
  }
}
