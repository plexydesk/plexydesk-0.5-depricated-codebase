<?php

class Plexydesk extends Controller {

	function Plexydesk()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Home";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->plexydesk();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>