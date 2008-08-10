<?php

	class Display extends Model 
	{
		
	    function Display()
	    {
	        parent::Model();
	    }
		function head()
		{
			$head_data['common_email'] = "suport@plexydesk.org";
			$this->load->view('front/common/main_menu', $head_data);	
		}
		function banner()
		{
			$this->load->view('front/common/banner');
		}
		function side_bar()
		{
			$this->load->view('front/common/side_bar');
		}
		function side_bar_plexy()
		{
			$this->load->view('front/common/side_bar_plexy');
		}
		function footer()
		{
			$this->load->view('front/common/page_footer');	
		}
	}
	
?>