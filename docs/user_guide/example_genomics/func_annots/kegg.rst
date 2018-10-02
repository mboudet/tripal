Adding KEGG Results
===================


What is KEGG?
--------------

 KEGG/KAAS (http://www.genome.ad.jp/tools/kaas/). The KAAS server receives as input a FASTA file of sequences and annotates those with KEGG orthologs and pathways.
The downloaded KEGG annotation file will look like this:

FILE GOES HERE.



The KEGG OBO
-------------

After installing the module, you will need to load the KEGG ontology itself.  To do so, go to ``admin -> Tripal -> Tripal Data Loaders -> Vocabularies -> OBO Vocabulary Loader``.  Select **KEGG** under **Ontology OBO File Reference** and press **Importer OBO File**.  Run the job to import the vocabulary:

::

  drush trp-run-jobs --username=administrator --root=$DRUPAL_HOME



Create a KEGG analysis
----------------------

.. note::

  It is always recommended to create an analysis page anytime you import data. The purpose of the analysis page is to describe how the data being added was derived or collected.

As with previous functional annotations, we start by creating an analysis. Navigate to **Content → Tripal Content** and click the **Add Tripal Content** link. This page contains a variety of content types that the site supports.  Scroll to the **Other** section and find the content type named **KEGG results**:

.. csv-table::
  :header: "Field", "Value"

    "Name", "KEGG analysis of Citrus sinensis v1.0"
    "Description", "Materials & Methods: C. sinensis mRNA sequences were uploaded to the KEGG Automatic Annotation Server (KAAS) where they were mapped to KEGG pathways and orthologs. The SBH (single-directional best hit) was used with the genes data set being the defaults for genes."
    "KEGG Program", "KEGG Automatic Annotation Server (KAAS)"
    "KEGG Version", "1.64a"
    "Data Source Name ", "C. sinensis v1.0 genes"
    "Date Performed", "(today's date)"

Click the **Save** button. You can now see our new KEGG Analysis page.


Run the KEGG importer
----------------------
The KEGG importer is available at ``Admin -> Tripal -> Data Loaders -> Chado KEGG Loader``.



The top section of this page provides multiple methods for providing results file: via an upload interface, specifying a remote URL or a file path that is local to the server.  Most likely, you will always upload or provide a remote URL.  However, we download the file earlier, and stored them here: ```$DRUPAL_HOME/sites/default/files```.  So, in this case we can use the path on the local server.  Provide the following value for this form:


.. csv-table::
  :header: "Field", "Value"

  "Server path", "sites/default/files/"
  "Analysis", "KEGG analysis of Citrus sinensis v1.0"
  "Query Name RE", ""
  "Use Unique Name", "Unchecked"
  "Query Type", "mRNA"

Click the **Import KEGG File** button.
