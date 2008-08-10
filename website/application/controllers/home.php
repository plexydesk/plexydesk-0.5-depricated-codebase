<?php

class Home extends Controller {

	function Home()
	{
		parent::Controller();	
	}
	
	function index()
	{	
		$page_data['page_title'] = "Home";
		
		$this->application->app_top($page_data);
		$this->display->head();
		$this->display->banner();
		$this->pages->home_page();
		$this->pages->home_side_bar();
		
		$this->display->footer();
	}
}
?>