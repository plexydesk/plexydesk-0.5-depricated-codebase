<?php

class Download extends Controller {

	function Download()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Dwonload";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->download();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>