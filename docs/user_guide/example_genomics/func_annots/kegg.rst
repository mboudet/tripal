Adding KEGG Results
===================


What is KEGG?
--------------

 KEGG/KAAS (http://www.genome.ad.jp/tools/kaas/). The KAAS server receives as input a FASTA file of sequences and annotates those with KEGG orthologs and pathways.


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

Click the **Save** button. You can now see our new BLAST analysis page.
