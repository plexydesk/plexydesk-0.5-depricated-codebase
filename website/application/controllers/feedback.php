<?php

class Feedback extends Controller {

	function Feedback()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Feedback";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->feedback();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>