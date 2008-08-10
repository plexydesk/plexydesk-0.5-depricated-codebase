<?php

	class Application extends Model 
	{
		
	    function Application()
	    {
	        parent::Model();
	    }
		function test()
		{
			echo "testing....";
		}
		function app_top($page_settings)
		{
			// meta tags
			$page_settings['description'] = "plexydesk blog";
			$page_settings['key_words'] = "plexydesk, c++, kde pluggings, ked desktop pluggings";
			$page_settings['author'] = "Hasanga Abeyaratne : 3dcreature@gmail.com";
			
			//page titles
			$page_settings['base_title'] = "PlexyDesk";
			
			$this->load->view('front/common/page_header', $page_settings);	
		}
		
		function footer_content()
		{			
			$this->load->view('common/footer');	
		}
	}
	
?>