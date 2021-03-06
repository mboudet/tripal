<?php

class operation__phylotree_vis extends ChadoField {

  // The default lable for this field.
  public static $default_label = 'Phylogenetic tree visualisation';

  // The default description for this field.
  public static $description = 'Rendering of a phylogenetic tree.';

  // Provide a list of instance specific settings. These can be access within
  // the instanceSettingsForm.  When the instanceSettingsForm is submitted
  // then Drupal with automatically change these settings for the instnace.
  // It is recommended to put settings at the instance level whenever possible.
  // If you override this variable in a child class be sure to replicate the
  // term_name, term_vocab, term_accession and term_fixed keys as these are
  // required for all TripalFields.
  public static $default_instance_settings  = array(
    // The short name for the vocabulary (e.g. shcema, SO, GO, PATO, etc.).
    'term_vocabulary' => 'operation',
    // The name of the term.
    'term_name' => 'Phylogenetic tree visualisation',
    // The unique ID (i.e. accession) of the term.
    'term_accession' => '0567',
    // Set to TRUE if the site admin is allowed to change the term
    // type. This will create form elements when editing the field instance
    // to allow the site admin to change the term settings above.
    'term_fixed' => FALSE,
  );

  // The default widget for this field.
  public static $default_widget = 'operation__phylotree_vis_widget';

  // The default formatter for this field.
  public static $default_formatter = 'operation__phylotree_vis_formatter';


  /**
   * @see TripalField::validate()
   */
  public function validate($entity_type, $entity, $langcode, $items, &$errors) {

    // If we don't have an entity then we don't want to validate.  The case
    // where this could happen is when a user is editing the field settings
    // and trying to set a default value. In that case there's no entity and
    // we don't want to validate.  There will always be an entity for creation
    // and update operations of a content type.
    if (!$entity) {
      return;
    }
    $settings = $this->field['settings'];
    $field_name = $this->field['field_name'];
    $field_type = $this->field['type'];
    $field_table = $this->instance['settings']['chado_table'];
    $field_column = $this->instance['settings']['chado_column'];

  }

  /**
   * @see TripalField::load()
   */
  public function load($entity) {

    $record = $entity->chado_record;
    $settings = $this->instance['settings'];

    $field_name = $this->field['field_name'];
    $field_type = $this->field['type'];
    $field_table = $this->instance['settings']['chado_table'];
    $field_column = $this->instance['settings']['chado_column'];

    // Get the terms for each of the keys for the 'values' property.
    $label_term = 'operation:0567';

    // Set some defaults for the empty record.
    $entity->{$field_name}['und'][0]['value'] = array();

    if ($record) {
      $entity->{$field_name}['und'][0]['value'] = array(
        'schema:url' => url('bio_data/' . $entity->id, array('absolute' => TRUE)),
      );
    }
  }

  /**
   * @see TripalField::elementInfo()
   */
  public function elementInfo() {
    $field_term = $this->getFieldTermID();

    return array(
      $field_term => array(
        'operations' => array(),
        'sortable' => FALSE,
        'searchable' => FALSE,
        'type' => 'string',
        'type' => 'xs:complexType',
        'readonly' => TRUE,
        'elements' => array(
          'schema:url' => array(
            'searchabel' => FALSE,
            'type' => 'xs:anyURI',
            'readonly' => TRUE,
            'required' => FALSE,
          ),
        ),
      )
    );
  }
}
