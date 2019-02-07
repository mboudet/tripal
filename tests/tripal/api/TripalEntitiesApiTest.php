<?php
namespace Tests\tripal\api;

use StatonLab\TripalTestSuite\DBTransaction;
use StatonLab\TripalTestSuite\TripalTestCase;

class TripalEntitiesApiTest extends TripalTestCase {
  // Uncomment to auto start and rollback db transactions per test method.
  use DBTransaction;

  /**
   * Test tripal_replace_entity_tokens().
   * @group Issue790
   */
  public function testReplaceEntityTokens() {

    // This is the string containing tokens.
    // The example is of an alias for a feature-based content type (e.g. gene).
    $string = 'features/[obi__organism,TAXRANK:0000005]/[obi__organism,TAXRANK:0000006]/[schema__name]';

    // Next, we need an entity.
    $organism = factory('chado.organism')->create(['genus' => Tripalus, 'species' => 'databasica']);
    $values = [
      'organism_id' => $organism->organism_id,
      'name' => 'Testingfb0dd78a-ada3-47ae-b412-3646e2f4f2a1',
    ];
    $entity = $this->getEntity('feature', $values);

    // Finally, we call the function we want to test.
    $result = tripal_replace_entity_tokens($string, $entity);

    // Check that the replaced string is what we expect.
    $expected_string = 'features/Tripalus/databasica/Testingfb0dd78a-ada3-47ae-b412-3646e2f4f2a1';
    $this->assertEquals($expected_string, $result, 'The resulting string from tripal_replace_entity_tokens() did not match what we expected.');
  }

  /**
   * Retrieve an entity of a given bundle.
   *
   * @param $base_table
   *   The base chado table for the entity (e.g. "feature" for a gene entity).
   * @param $values
   *   An array of values to pass to the factory when creating
   *   the record the entity will use.
   * @return
   *   A newly published entity.
   */
  public function getEntity($base_table, $values) {

    // Find a bundle which stores it's data in the given base table.
    // This will work on Travis since Tripal creates matching bundles by default.
	  $bundle_details = db_query("
      SELECT bundle_id, type_column, type_id
      FROM chado_bundle b
      WHERE data_table=:table AND type_linker_table=''
      ORDER BY bundle_id ASC LIMIT 1", [':table' => $base_table]
	  )->fetchObject();
	  if (isset($bundle_details->bundle_id)) {
		  $bundle_id = $bundle_details->bundle_id;
		}

    $bundle_name = 'bio_data_' . $bundle_id;

		// Create an entity so that we know there are some available to find.
    if ($bundle_details->type_column == 'type_id') {
	    $values['type_id'] = (isset($values['type_id'])) ? $values['type_id'] : $bundle_details->type_id;
	    $chado_record = factory('chado.' . $base_table)->create($values);
		}
		else {
		  $chado_record = factory('chado.' . $base_table)->create($values);
		}
		// Then publish them so we have entities.
		$this->publish($base_table);

    // Find our fake entity from the above bundle.
	  $entity_id = db_query(
		  'SELECT entity_id FROM chado_' . $bundle_name
			. ' WHERE record_id=:chado_id',
			[':chado_id' => $chado_record->{$base_table . '_id'}]
		)->fetchField();
		$entities = entity_load('TripalEntity', [$entity_id]);

		return array_pop($entities);;
  }
}
