<?php

class Screenshots extends Controller {

	function Screenshots()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Screenshots";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->screenshots();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>