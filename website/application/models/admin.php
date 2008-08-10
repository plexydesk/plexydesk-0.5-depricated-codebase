<?php

	class Admin extends Model 
	{
		
	    function Admin()
	    {
	        parent::Model();
	    }
		function post_entry()
		{
			$this->load->view('front/blog/post_entry');
		}
		function new_cat()
		{
			$this->load->view('back/blog/new_category');
		}
	}
	
?>