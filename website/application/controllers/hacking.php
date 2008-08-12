<?php

class Hacking extends Controller {

	function Hacking()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Hacking";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->hacking();
		$this->display->side_bar();
		
		$this->display->footer();
	}
}
?>