<?php

class Ideas extends Controller {

	function Ideas()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Dwonload";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->ideas();
		$this->display->side_bar();
		
		$this->display->footer();
	}
}
?>