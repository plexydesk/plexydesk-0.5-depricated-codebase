<?php

class Sponcer extends Controller {

	function Sponcer()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Sponcer";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->sponcer();
		$this->display->side_bar_plexy();
		
		$this->display->footer();
	}
}
?>