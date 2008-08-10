	<br class="clear">
	<div id="content">
		<div id="main">
			<div class="main-block">
				<h1>Hacking</h1>
				<h2>Coding Style</h2>
				<h3>Formatting rules</h3>
				<ul class="normal-list">
					<li>Use 4 spaces to indent and do not use tabs. the length of a line should be 80 chars wide.</li>
					<li>When the lines are longer than 80 start a new line.</li>
					<li>Put spaces between operators and arguments.</li>
					<li>Put spaces between brackets and arguments of functions.</li>
					<li>For pointer and reference variable declarations put a space between the type and the or & and no space before the variable name.</li>
					<li>For if, else, while and similar statements put the brackets on the same line as the statement.</li>
					<li>Function and class definitions have their brackets on separate lines.</li>
					<li>For class, variable, function names separate multiple words by upper-casing the words preceded by other words.</li>
					<li>Make sure that source file hasn’t useless black spaces.</li>
				</ul>
				<h3>Class Names</h3>
				<ul class="normal-list">
					<li>Class names start with an upper-case letter. <br/ > example : class Cookie {}; </li>
				</ul>
				<h3>Variable Names</h3>
				<ul class="normal-list">
					<li>Function names start with a lower-case letter.</li>
					<li>Use functions that makes sense and has logical sense. <br /> example : example : fooBar () </li>
				</ul>
				<h3>Header Formats</h3>
				<ul class="normal-list">
					<li>General formatting rules apply.</li>
					<li>Access modifiers are indented.</li>
					<li>Put curly brackets of class definitions on its own line.</li>
					<li>Double inclusion protection defines are all upper case letters and are composed of the namespace (if available), the classname and a H suffix separated by underscores.</li>
					<li>Inside a namespace there is no indentation.</li>
				</ul>
				<h3>Comment styles</h3>
				<ul class="normal-list">
					<li>Each public function must have a Doxygen compatible comment in the header.</li>
				</ul>
				<h3>Misc</h3>
				<ul class="normal-list">
					<li>Use true or false to booleans and 0 for null pointers.</li>
				</ul>
				<h3>Automating Indentation</h3>
				<ul class="normal-list">
					<li>use GNU indent to batch indent source files, you may use these options “indent -nprs -nbap -bap -bc -bad -bap -bbb -br -lp -l80 -nut -i4 -nce”. But we don’t recommend the use of this tool as we have seen it has bugs which results in inconstant styles. So the best option is to make sure you follow the rules when you code, and correct when you spot a code segment that breaks it.</li>
				</ul>
				<h2>SVN commit Guide</h2>
				<ul class="list">
					<li><b>How to Apply : </b> Email one of the core members or ask about it on the mailing list.</li>
					<li><b>Before you commit : </b> Make sure the code matches the hacking guidelines, and it compiles with a known compiler.</li>
					<li class="delete"><b>What you shouldn’t commit : </b> temporary files and hidden files. this can happen when you try to import or add a new directory. then also,Do not commit copyrighted images or similar. since plexy is a GPL project please also attach a copy right notice for each binary file you will commit to the repository.</li>
					<li><b>When and When not to commit : </b> When there is a freez announced please do not commit. this can mess up the process. if the commit is a must, then talk about it on the mailing list or irc before doing so.</li>
				</ul>
				<p>
					Some parts taken of KDE PIM Hacking Guide.
				</p>
			</div>
		</div>
