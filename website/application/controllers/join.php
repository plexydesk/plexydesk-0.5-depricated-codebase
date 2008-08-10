<?php

class Join extends Controller {

	function Ideas()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Join With Plexydesk";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->join();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>