<?php

class Help_us extends Controller {

	function Help_us()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Help";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->help_us();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>