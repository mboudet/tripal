<?php

class TripalWebServiceCollection extends TripalWebServiceResource {

  /**
   * Holds the data portion of the JSON-LD response if this resource is
   * a collection.
   */
  protected $members;

  /**
   * Set to TRUE if paging should be enabled.  Paging is disabled by default.
   */
  protected $doPaging;

  /**
   * The total number of items.  This variable is used if paging is turned on.
   * Otherwise it's ignored and the total number of items reported by the
   * collection will be the number of elements in the $members array.
   */
  protected $totalItems;

  /**
   * When the collection contains more than the itemsPerpage amount then
   * the results will be paged.
   */
  protected $itemsPerPage;

  /**
   * The current page of the pager.
   */
  protected $page;


  /**
   * Implements the constructor.
   *
   * @param TripalWebService $service
   *   An instance of a TripalWebService or class that extends it.
   */
  public function __construct($service_path) {
    parent::__construct($service_path);
    $this->members = array();
    $term = tripal_get_term_details('hydra', 'Collection');
    $this->addContextItem('Collection', $term['url']);
    $term = tripal_get_term_details('hydra', 'totalItems');
    $this->addContextItem('totalItems', $term['url']);
    $term = tripal_get_term_details('hydra', 'member');
    $this->addContextItem('member', $term['url']);
    parent::setType('Collection');

    // If the totalItems is set to -1 then this means paging is turned off and
    // all of the elements in the $memgbers array should be used.
    $this->totalItems = 0;
    $this->itemsPerPage = 25;
    $this->doPaging = FALSE;
  }

  /**
   * Initializes the pager.
   *
   * @param $totalItems
   *   The total number of items available.
   * @param $itemsPerPage
   *   The maximum number of items per page.
   * @param $path
   *   The path
   */
  public function initPager($totalItems, $itemsPerPage, $page) {
    $this->doPaging = TRUE;
    $this->totalItems = $totalItems;
    $this->itemsPerPage = $itemsPerPage;
    $this->page = $page;
  }

  /**
   * Adds a new member to this resource if it is a collection.
   *
   * @param $member
   *   A TripalWebServiceResource member whose type is the same as this
   *   resource
   */
  public function addMember($member) {
    // Make sure the $servcie provides is a TripalWebServcie class.
    if (!is_a($member, 'TripalWebServiceResource')) {
      throw new Exception("Cannot add a new member to this resource collection as it is not a TripalWebServiceResource.");
    }
    $this->members[] = $member;
  }

  /**
   * @see TripalWebServiceResource::setType()
   */
  public function setType($type) {
    throw new Exception("The type for a Collection can only be collection.");
  }

  /**
   * Retrieves the data section of the resource.
   *
   * The JSON-LD response constists of two sections the '@context' section
   * and the data section.  This function only returns the data section
   * for this resource
   *
   * @return
   *   An associative array containing the data section of the response.
   */
  public function getData() {
    $data = $this->data;
    $data['totalItems'] = 0;

    if ($this->doPaging == TRUE) {

      $data['totalItems'] = $this->totalItems;
      $total_pages = ceil($this->totalItems / $this->itemsPerPage);
      $page = $this->page;
      $limit = $this->itemsPerPage;

      if ($this->totalItems > 0) {
        $data['view'] = array(
          '@id' => $this->service_path . '?' . implode('&', array_merge(array("page=$page", "limit=$limit"))),
          '@type' => 'PartialCollectionView',
          'first' => $this->service_path . '?' . implode('&', array_merge(array("page=1", "limit=$limit"))),
          'last' => $this->service_path . '?' . implode('&', array_merge(array("page=$total_pages", "limit=$limit"))),
        );
        $prev = $page - 1;
        $next = $page + 1;
        if ($prev > 0) {
          $data['view']['previous'] = $this->service_path .'?' . implode('&', array("page=$prev", "limit=$limit"));
        }
        if ($next < $total_pages) {
          $data['view']['next'] = $this->service_path . '?' . implode('&', array("page=$next", "limit=$limit"));
        }
      }
    }
    else {
      $data['totalItems'] = count($this->members);
    }

    $member_data = array();
    foreach ($this->members as $key => $member) {
      $member_data[] = $member->getData();
    }
    $data['members'] = $member_data;

    // If paging of this collection is enabled then add the pager control links.

    return $data;
  }

  /**
   * Retrieves the data section of the resource.
   *
   * The JSON-LD response constists of two sections the '@context' section
   * and the data section.  This function only returns the data section
   * for this resource
   *
   * @return
   *   An associative array containing the data section of the response.
   */
  public function getContext() {
    if ($this->doPaging == TRUE) {
      $this->addContextItem('view', 'hydra:PartialCollectionView');
    }
    $context = $this->context;
    foreach ($this->members as $key => $member) {
      $citems = $member->getContext();
      foreach ($citems as $key => $val) {
        $context[$key] = $val;
      }
    }
    return $context;
  }
}