<?php

class so__transcript extends ChadoField {


  // --------------------------------------------------------------------------
  //                     EDITABLE STATIC CONSTANTS
  //
  // The following constants SHOULD be set for each descendent class.  They are
  // used by the static functions to provide information to Drupal about
  // the field and it's default widget and formatter.
  // --------------------------------------------------------------------------

  // The default lable for this field.
  public static $default_label = 'Transcripts';

  // The default description for this field.
  public static $description = 'An RNA synthesized on a DNA or RNA template by an RNA polymerase.';

  // Provide a list of instance specific settings. These can be access within
  // the instanceSettingsForm.  When the instanceSettingsForm is submitted
  // then Drupal with automatically change these settings for the instnace.
  // It is recommended to put settings at the instance level whenever possible.
  // If you override this variable in a child class be sure to replicate the
  // term_name, term_vocab, term_accession and term_fixed keys as these are
  // required for all TripalFields.
  public static $default_instance_settings  = array(
    // The short name for the vocabulary (e.g. shcema, SO, GO, PATO, etc.).
    'term_vocabulary' => 'SO',
    // The name of the term.
    'term_name' => 'transcript',
    // The unique ID (i.e. accession) of the term.
    'term_accession' => '0000673',
    // Set to TRUE if the site admin is allowed to change the term
    // type. This will create form elements when editing the field instance
    // to allow the site admin to change the term settings above.
    'term_fixed' => TRUE,
  );

  // The default widget for this field.
  public static $default_widget = 'so__transcript_widget';

  // The default formatter for this field.
  public static $default_formatter = 'so__transcript_formatter';

  // --------------------------------------------------------------------------
  //              PROTECTED CLASS MEMBERS -- DO NOT OVERRIDE
  // --------------------------------------------------------------------------
  // An array containing details about the field. The format of this array
  // is the same as that returned by field_info_fields()
  protected $field;
  // An array containing details about an instance of the field. A field does
  // not have to have an instance.  But if dealing with an instance (such as
  // when using the widgetForm, formatterSettingsForm, etc.) it should be set.
  protected $instance;


  /**
   *
   * @see TripalField::validate()
   */
  public function validate($entity_type, $entity, $field, $items, &$errors) {

  }


  /**
   *
   * @see TripalField::load()
   */
  public function load($entity, $details = array()) {

    $record = $details['record'];
    $field_name = $this->field['field_name'];
    $field_type = $this->field['type'];
    $field_table = $this->instance['settings']['chado_table'];
    $field_column = $this->instance['settings']['chado_column'];

    // Set some defaults for the empty record.
    $entity->{$field_name}['und'][0] = array(
      'value' => array(
        'type' => '',
        'name' => '',
        'identifier' => '',
        'location' => '',
      ),
    );

    // Get the mRNA features for this gene.
    $sql = "
      SELECT FS.name, FS.uniquename, FS.feature_id, FCVT.name as type
      FROM {feature_relationship} FR
        INNER JOIN {feature} FS on FS.feature_id = FR.subject_id
        INNER JOIN {cvterm} FCVT on FCVT.cvterm_id = FS.type_id
        INNER JOIN {cv} CV on CV.cv_id = FCVT.cv_id
      WHERE
        FR.object_id = :feature_id and
        FCVT.name = 'mRNA' and
        CV.name = 'sequence'
    ";
    $results = chado_query($sql, array(':feature_id' => $record->feature_id));
    $i = 0;
    while ($transcript = $results->fetchObject()) {
      // Get the location of this mRNA.
      $sql = "
        SELECT FL.*, F.name as srcfeature_name
        FROM {featureloc} FL
          INNER JOIN {feature} F on F.feature_id = FL.srcfeature_id
        WHERE FL.feature_id = :object_id
      ";
      $floc_results = chado_query($sql, array(':object_id' => $transcript->feature_id));
      $loc = "";
      while ($location = $floc_results->fetchObject()) {
        $loc .= $location->srcfeature_name . ":" . $location->fmin . ".." . $location->fmax;
      }
      $entity->{$field_name}['und'][$i]['value'] = array(
        'type' => $transcript->type,
        'name' => $transcript->name,
        'identifier' => $transcript->uniquename,
        'location' => $loc,
      );
      $entity_id = tripal_get_chado_entity_id($field_table, $record->feature_id);
      if ($entity_id) {
         $entity->{$field_name}['und'][$i]['value']['entity'] = 'TripalEntity:' . $entity_id;
       }
      $i++;
    }
  }

}