<?php

/**
 * @file
 * Controlled Vocabulary API
 *
 * @defgroup tripal_cv_api CV Module API
 * @ingroup tripal_api
 * @ingroup tripal_cv
 * This module provides a set of functions to simplify working with
 * controlled vocabularies.  Most of the API functions deal with retrieving
 * terms or their parent vocabularies.
 *
 * However, the API also supports
 * generation of trees for browsing a vocabulary as well as generation of
 * pie graphs for display of hierarchical counts of terms.  Version 0.3b of
 * Tripal provides a feature browser and a feature summary chart uses
 * the API functions provided here.  But in general charts and trees can be
 * created for any controlled vocabulary.
 *
 */

/**
 * Purpose: To retrieve a chado controlled vocabulary object
 *
 * @param $select_values
 *   An array meant to uniquely select a given controlled vocabulary
 *
 * @return
 *   Chado controlled vocabulary object
 *
 * The controlled vocabulary is selected using tripal_core_chado select and as such the
 * $select_values array parameter meant to uniquely identify the controlled vocab to be
 * returned follows the same form as when using tripal_core_chado_select directly.
 *
 * Example Usage:
 * @code
    $select_values = array(
      'name' => 'feature_property'
    );
    $cv_object = tripal_cv_get_cv($select_values);
 * @endcode
 *  The above code selects the feature_property cv and returns the following object:
 * @code
    $cv_object = stdClass Object (
      [cv_id] => 13
      [name] => feature_property
      [definition] =>
    );
 * @endcode
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_get_cv($select_values) {

  $columns = array(
    'cv_id',
    'name',
    'definition',
  );
  $results = tripal_core_chado_select('cv', $columns, $select_values);
  if (sizeof($results) == 1) {
    return $results[0];
  }
  elseif (empty($results)) {
    watchdog('tripal_cv',
      'tripal_cv_get_cv: No cv matches criteria values:%values',
      array('%values' => print_r($select_values, TRUE)),
      WATCHDOG_WARNING
    );
    return FALSE;
  }
  else {
    watchdog('tripal_cv',
      'tripal_cv_get_cv: 2+ cvs match criteria values:%values',
      array('%values' => print_r($select_values, TRUE)),
      WATCHDOG_WARNING
    );
  }

}

// Purpose: To retrieve a chado cv object
// @param $where_options
//   @code
//        array(
//                          <column_name> => array(
//                            'type' => <type of column: INT/STRING>,
//                            'value' => <the vlaue you want to filter on>,
//                            'exact' => <if TRUE use =; if FALSE use ~>,
//                          )
//        )
// @endcode
//
// @return
//   Chado cv object with all fields from the chado cv table
//
// @ingroup tripal_cv_api
//
//function tripal_cv_get_cv ($where_options)

/**
 * Retrieve a cv given the cv name
 *
 * @param $name
 *  The name of the cv to be returned
 * @return
 *   The cv object for the specified CV name
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_get_cv_by_name($name) {

  $previous_db = tripal_db_set_active('chado');
  $r = tripal_core_chado_select('cv', array('*'), array('name' => $name));
  tripal_db_set_active($previous_db);

  return $r[0];
}

/**
 * Retrieve the cv object for the specified CV id
 *
 * NOTE: This function is deprecated.
 * @see tripal_core_chado_generate_vars()
 *
 * @param $cv_id
 *   The unique identifier for the cv retrieve
 *
 * @return
 *   An object describing the cv
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_get_cv_by_id($cv_id) {

  $previous_db = tripal_db_set_active('chado');
  $r = tripal_core_chado_select('cv', array('*'), array('cv_id' => $cv_id));
  tripal_db_set_active($previous_db);

  return $r;
}

/**
 * Create an options array to be used in a form element which provides a list of all chado cvs
 *
 * @return
 *   An array(cv_id => name) for each cv in the chado cv table
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_get_cv_options() {

  $results = tripal_core_chado_select('cv', array('cv_id', 'name'), array());

  $options = array();
  foreach ($results as $r) {
    $options[$r->cv_id] = $r->name;
  }

  return $options;

}

/**
 * Retrieve a chado cvterm object with a given name
 *
 * @param $name
 *   the cvterm.name
 * @param $cv_id
 *   the cv_id of the term you are looking for
 * @param $cv_name
 *   the name of the CV
 *
 * @return
 *   cvterm object
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_get_cvterm_by_name($name, $cv_id = 0, $cv_name = 'tripal') {

  if ($cv_id) {
    $values = array(
       'name' => $name,
       'cv_id' => $cv_id,
    );
    $r = tripal_core_chado_select('cvterm', array('*'), $values);
  }
  elseif ($cv_name) {
    $values = array(
      'name' => $name,
      'cv_id' => array(
        'name' => $cv_name,
      ),
    );
    $r = tripal_core_chado_select('cvterm', array('*'), $values);
  }
  else {
    $values = array(
      'name' => $name,
    );
    $r = tripal_core_chado_select('cvterm', array('*'), $values);
  }

  if (!$r) {
    return FALSE;
  }
  if (count($r) > 0) {
    return FALSE;
  }
  return $r[0];
}

/**
 * Create an options array to be used in a form element
 *   which provides a list of all chado cvterms
 *
 * @param $cv_id
 *   The chado cv_id;
 *   only cvterms with the supplied cv_id will be returned
 * @return
 *   An array(cvterm_id => name)
 *   for each cvterm in the chado cvterm table where cv_id=that supplied
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_get_cvterm_options($cv_id = 0) {

  if ($cv_id > 0) {
    $results = tripal_core_chado_select('cvterm', array('cvterm_id', 'name'), array('cv_id' => $cv_id));
  }
  else {
    $results = tripal_core_chado_select('cvterm', array('cvterm_id', 'name'), array());
  }

  $options = array();
  foreach ($results as $r) {
    $options[$r->cvterm_id] = $r->name;
  }

  return $options;

}



/**
 * Adds a controlled vocabular to the CV table of Chado.
 *
 * @param $name
 *   The name of the controlled vocabulary. These are typically all lower case
 *   with no special characters other than an undrescore (for spaces).
 * @param $comment
 *   A description or definition of the vocabulary.
 *
 * @return
 *   An object populated with fields from the newly added database.
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_add_cv($name, $definition) {
	
	$ins_values = array(
	  'name'       => $name,
	  'definition' => $definition
	);
	
  // see if the CV (default-namespace) exists already in the database
  $sel_values = array('name' => $name);
  $sel_options = array('statement_name' => 'sel_cv_na');
  $results = tripal_core_chado_select('cv', array('*'), $sel_values, $sel_options);

  // if it does not exists then add it
  if (count($results) == 0) {  	
    $ins_options = array('statement_name' => 'ins_cv_nade');
  	$success = tripal_core_chado_insert('cv', $ins_values, $ins_options);
    if (!$success) {
      watchdog('tripal_cv', "Failed to create the CV record", NULL, WATCHDOG_WARNING);
      return FALSE;
    }
    $results = tripal_core_chado_select('cv', array('*'), $sel_values, $sel_options);
  }
  // if it already exists then do an update
  else {
    $upd_options = array('statement_name' => 'upd_cv_nade');
  	$success = tripal_core_chado_update('cv', $sel_values, $ins_values, $upd_options);
    if (!$success) {
      watchdog('tripal_cv', "Failed to update the CV record", NULL, WATCHDOG_WARNING);
      return FALSE;
    }
    $results = tripal_core_chado_select('cv', array('*'), $sel_values, $sel_options);
  }

  // return the cv object
  return $results[0];
}

/**
 *  Add's a CV term to the cvterm table.  If the parent CV does not exist then
 *  that too is added to the CV table.  If the cvterm is a relationship term
 *  then the $is_relationship argument should be set.  The function will try
 *  to first find the relationship in the relationship ontology for updating and
 *  if it can't be found will add the relationship to the __global CV.  All terms
 *  must also have a corresponding database.  This is specified in the term's
 *  ID just before the colon (e.g. GO:003824).  If the database does not exist
 *  in the DB table then it will be added automatically.  The accession (the
 *  value just after the colon in the term's ID) will be added to the dbxref
 *  table.  If the CVterm already exists and $update is set (default) then the
 *  cvterm is updated.  If the CVTerm already exists and $update is not set, then
 *  no changes are made and the CVTerm object is returned.
 *
 * @param $term
 *   An associative array with the following keys: 'id', 'name' and 'namespace',
 *   'is_obsolete', and 'def'.  Where 'id' is the term accession, 'name' is the
 *   term name, 'namespace' is the CV name for the term, 'def' is the term
 *   definition and 'is_obsolete' is present and set to 1 if the term is defunct.
 *   The 'id' must be of the form <DB>:<ACCESSION>, where <DB> is the name of
 *   the database to which the cvterm belongs and the <ACCESSION> is the
 *   term's accession number in the database.
 * @param $defaultcv
 *   Optional. The CV name to which the term
 *   belongs.  If this arugment is null or not provided then the function tries
 *   to find a record in the CV table with the same name provided in the
 *   $term[namespace].  If this field is provided then it overrides what the
 *   value in $term[namespace]
 * @param $is_relationship
 *   If this term is a relationship term then this value should be 1.
 * @param $update
 *   By default this is set to 1.  If the term exists it is automatically updated.
 * @param $dbname
 *   In some cases the database name will not be part of the $term['id'] and it
 *   needs to be explicitly set.  Use this argument only if the database name
 *   cannot be specififed in the term ID (e.g. <DB>:<ACCESSION>).
 *
 * @return
 *   A CVTerm object
 *
 * @ingroup tripal_cv_api
 */
function tripal_cv_add_cvterm($term, $defaultcv = '_global', $is_relationship = 0, 
  $update = 1, $dbname = 'internal') {
  	
  $connection = tripal_db_persistent_chado();
  	
  // get the term properties
  $id = $term['id'];
  $name = $term['name'];
  $cvname = $term['namespace'];
  $definition = preg_replace('/^\"(.*)\"/', '\1', $term['def']);
  $is_obsolete = 0;
  
  if (isset($term['is_obsolete']) and strcmp($term['is_obsolete'], 'true') == 0) {
    $is_obsolete = 1;
  }  
  if (!$name and !$id) {
    watchdog('tripal_cv', "Cannot find cvterm without 'id' or 'name'", NULL, WATCHDOG_WARNING);
    return 0;
  }
  if (!$id) {
    $id = $name;
  }
  if (!$name) {
    $name = $id;
  }
  if (!$cvname) {
    $cvname = $defaultcv;
  }
  
  // get the accession and the database from the cvterm id
  if ($dbname) {
    $accession = $id;
  }
  
  if (preg_match('/^.+?:.*$/', $id)) {
    $accession = preg_replace('/^.+?:(.*)$/', '\1', $id);
    $dbname = preg_replace('/^(.+?):.*$/', '\1', $id);
  }
  
  // check that we have a database name, give a different message if it's a relationship
  if ($is_relationship and !$dbname) {
    watchdog('tripal_cv', "A database name is not provided for this relationship term: $id", NULL, WATCHDOG_WARNING);
    return 0; 
  }
  if (!$is_relationship and !$dbname) {
    watchdog('tripal_cv', "A database identifier is missing from the term: $id", NULL, WATCHDOG_WARNING);
    return 0;
  }
  
  // make sure the CV name exists
  $cv = tripal_cv_add_cv($cvname, '');
  if (!$cv) {
    watchdog('tripal_cv', "Cannot find namespace '$cvname' when adding/updating $id", NULL, WATCHDOG_WARNING);
    return 0;
  }

  // this SQL statement will be used a lot to find a cvterm so just set it
  // here for easy reference below.  Because CV terms can change their names
  // but accessions don't change, the following SQL finds cvterms based on
  // their accession rather than the name
  if (!tripal_core_is_sql_prepared('sel_cvterm_by_accession')) {
	  $pcvtermsql = "
	    PREPARE sel_cvterm_by_accession(text, text) AS
	    SELECT CVT.name, CVT.cvterm_id, CV.cv_id, CV.name as cvname, 
	      DB.name as dbname, DB.db_id, DBX.accession 
	    FROM cvterm CVT
	      INNER JOIN dbxref DBX on CVT.dbxref_id = DBX.dbxref_id
	      INNER JOIN db DB on DBX.db_id = DB.db_id
	      INNER JOIN cv CV on CV.cv_id = CVT.cv_id
	    WHERE DBX.accession = $1 and DB.name = $2";
	  if(!chado_query($pcvtermsql)){
	    watchdog('tripal_cv', "Cannot prepare statement 'sel_cvterm_by_accession'", NULL, WATCHDOG_WARNING);
      return 0; 
	  }
  } 
  $cvtermsql = "EXECUTE sel_cvterm_by_accession('%s','%s')";  
  
  // add the database. The function will just return the DB object if the
  // database already exists.
  $db = tripal_db_add_db($dbname);
  if (!$db) {
    watchdog('tripal_cv', "Cannot find database '$dbname' in Chado.", NULL, WATCHDOG_WARNING);
    return 0;
  }

  // the cvterm table has two unique dependencies. We need to check both.
  // first check the (name, cv_id, is_obsolete) constraint
  $values = array(
     'name' => $name,
     'is_obsolete' => $is_obsolete,
     'cv_id' => array(
       'name' => $cvname,
     ),
  );
  $options = array('statement_name' => 'sel_cvterm_c1');
  $result = tripal_core_chado_select('cvterm', array('*'), $values, $options);
  
  // if the constraint is met then let's check it to see if
  // the database name matches the one we have been provided
  if (count($result) == 1) {
  	$cvterm = $result[0];
  	
  	// get the dbxref
    $values = array('dbxref_id' => $cvterm->dbxref_id);
    $options = array('statement_name' => 'sel_dbxref_id');
    $result = tripal_core_chado_select('dbxref', array('*'), $values, $options);
    $dbxref = $result[0];
    
    // get the db
    $values = array('db_id' => $dbxref->db_id);
    $options = array('statement_name' => 'sel_db_id');
    $result = tripal_core_chado_select('db', array('*'), $values, $options);
    $db_check = $result[0];
    
    // the database name for this existing term does not match that of the 
    // one provided to this function.  The CV name matches otherwise we
    // wouldn't have made it this far. So, let's swap the database for
    // this term
    if ($db_check->name != $db->name) {
    	
    	// look to see if the correct dbxref record already exists for 
    	// this database
    	$values = array(
    	  'db_id' => $db->db_id,
    	  'accession' => $accession,    	 
    	);
    	$options = array('staement_name' => 'sel_dbxref_idac');
    	$result = tripal_core_chado_select('dbxref', array('*'), $values, $options);
    	
    	// if we already have a good dbxref then we want to update our cvterm to use this dbxref
    	if (count($result) > 0) {
    		$dbxref = $result[0];
    	  $match = array('cvterm_id' => $cvterm->cvterm_id);
	      $values = array('dbxref_id' => $dbxref->dbxref_id);
	      $options = array('statement_name' => 'upd_cvterm_db');
	      $success = tripal_core_chado_update('cvterm', $match, $values, $options); 
	      if (!$success) {
	        watchdog('tripal_cv', "Failed to correct the dbxref id for the $cvterm, " .
	          "$name (id: $accession), for database $dbname", NULL, WATCHDOG_WARNING);
	        return 0;
	      }
    	}
    	// if we don't have the record then we want to delete our cvterm and let the code
    	// below recreate it with the correct info 
    	else {
        $match = array('cvterm_id' => $cvterm->cvterm_id);
    		tripal_core_chado_delete('cvterm', $match);
    	}    	
    }
    
    // check that the accession matches.  Sometimes an OBO can define the same term 
    // multiple times but with different accessions.  If this is the case we
    // can't do an insert or it will violate the constraint in the cvterm table.
    // so we'll need to add the record to the cvterm_dbxref table instead
    if ($dbxref->accession != $accession) {
    	
    	// get/add the dbxref fort his term
      $dbxref_new =  tripal_db_add_dbxref($db->db_id, $accession);
      if (!$dbxref_new) {
        watchdog('tripal_cv', "Failed to find or insert the dbxref record for cvterm, " .
          "$name (id: $accession), for database $dbname", NULL, WATCHDOG_WARNING);
        return 0;
      }
      
      // check to see if the cvterm_dbxref record already exists
      $values = array(
        'cvterm_id' => $cvterm->cvterm_id,
        'dbxref_id' => $dbxref_new->dbxref_id,
        'is_for_definition' => 1,
      );
      $options = array('statement_name' => 'sel_cvtermdbxref_cvdbis');      
      $result = tripal_core_chado_select('cvterm_dbxref', array('*'), $values, $options);
      
      // if the cvterm_dbxref record does not exists then add it 
      if (count($result)==0) {
      	$options = array('statement_name' => 'ins_cvtermdbxref_cvdbis');
	      $success = tripal_core_chado_insert('cvterm_dbxref', $values, $options);
	      if(!$success){
	        watchdog('tripal_cv', "Failed to find or insert the cvterm_dbxref record for a ". 
	          "duplicated cvterm:  $name (id: $accession), for database $dbname", NULL, WATCHDOG_WARNING);
	        return 0;
	      }
      }	      
	    // get the original cvterm with the same name and return that.
	    $cvterm = db_fetch_object(chado_query($cvtermsql, $dbxref->accession, $dbname));
	    return $cvterm;
    }
    
    // continue on, we've fixed the record if the db_id did not match, 
    // we can now perform and updated if we need to.
  }
  
  // get the CVterm record
  $cvterm = db_fetch_object(chado_query($cvtermsql, $accession, $dbname)); 
  //print "$pcvtermsql\n$cvtermsql\n$accession, $dbname\n";
  //print "CVTERM:\n";
  //print_r($cvterm);
  if (!$cvterm) {

  	// check to see if the dbxref exists if not, add it
    $dbxref =  tripal_db_add_dbxref($db->db_id, $accession);
    if (!$dbxref) {
      watchdog('tripal_cv', "Failed to find or insert the dbxref record for cvterm, " .
        "$name (id: $accession), for database $dbname", NULL, WATCHDOG_WARNING);
      return 0;
    }
    
    // check to see if the dbxref already has an entry in the cvterm table
    // this is the second constraint in the cvterm table
    $values = array('dbxref_id' => $dbxref->dbxref_id);
    $options = array('statement_name' => 'sel_cvterm_db');
    $check = tripal_core_chado_select('cvterm', array('cvterm_id'), $values, $options);
    if (count($check) == 0) {    	
      // now add the cvterm
      $ins_values = array(
        'cv_id'                => $cv->cv_id,
        'name'                 => $name,
        'definition'           => $definition,
        'dbxref_id'            => $dbxref->dbxref_id,
        'is_obsolete'          => $is_obsolete,
        'is_relationshiptype'  => $is_relationship, 
      );
      $ins_options = array('statement_name' => 'ins_cvterm_all');
      $success = tripal_core_chado_insert('cvterm', $ins_values, $ins_options);
      if (!$success) {
        if (!$is_relationship) {
          watchdog('tripal_cv', "Failed to insert the term: $name ($dbname)", NULL, WATCHDOG_WARNING);
          return 0;
        }
        else {
          watchdog('tripal_cv', "Failed to insert the relationship term: $name (cv: " . $cvname . " db: $dbname)", NULL, WATCHDOG_WARNING);
          return 0;
        }
      }
    }
    // this dbxref already exists in the cvterm table 
    else {
      watchdog('tripal_cv', "The dbxref already exists for another cvterm record: $name (cv: " . $cvname . " db: $dbname)", NULL, WATCHDOG_WARNING);
      return 0;
    }    
    $cvterm = db_fetch_object(chado_query($cvtermsql, $accession, $dbname));
  }
  // upate the cvterm
  elseif ($update) { 
  	$match = array('cvterm_id' => $cvterm->cvterm_id);
  	$upd_values = array(
        'name'                => $name,
        'definition'          => $definition,
        'is_obsolete'         => $is_obsolete,
        'is_relationshiptype' => $is_relationship, 
    );
    $upd_options = array('statement_name' => 'upd_cvterm_nadeisis');
    $success = tripal_core_chado_update('cvterm', $match, $upd_values, $upd_options);    
    if (!$success) {
      watchdog('tripal_cv', "Failed to update the term: $name", NULL, WATCHDOG_WARNING);
      return FALSE;
    }
    $cvterm = db_fetch_object(chado_query($cvtermsql, $accession, $dbname));
  } 
  else {
     // do nothing, we have the cvterm but we don't want to update
  }
  // return the cvterm
  return $cvterm;
}

