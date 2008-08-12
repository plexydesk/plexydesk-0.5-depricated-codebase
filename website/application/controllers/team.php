<?php

class Team extends Controller {

	function Team()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "PlexyDesk Team";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->pages->team();
		$this->display->side_bar();
		
		$this->display->footer();
	}
}
?>