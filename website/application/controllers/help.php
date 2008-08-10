<?php

class Help extends Controller {

	function Help()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Help";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->help();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>