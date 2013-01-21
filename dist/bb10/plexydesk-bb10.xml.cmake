<qnx>
        <id>org.plexydesk.bb10</id>
        <name>PlexyDesk</name>
        <versionNumber>${APPLICATION_MAIN_VERSION}</versionNumber>
        <description>Sync Service for PlexyDesk</description>
        <initialWindow>
            <systemChrome>standard</systemChrome>
            <transparent>false</transparent>
            <autoOrients>false</autoOrients>
            <aspectRatio>portrait</aspectRatio>
        </initialWindow>
        <env value="app/native/imports" var="QML_IMPORT_PATH"/>
        <env value="app/native/qml" var="QML2_IMPORT_PATH"/>
        <env value="app/native/plugins:/usr/lib/qt4/plugins" var="QT_PLUGIN_PATH"/>
        <env value="app/native/lib:/usr/lib/qt4/lib/" var="LD_LIBRARY_PATH"/>
        <arg>-platform</arg>
        <arg>blackberry</arg>
        <action system="true">run_native</action>
        <asset path="bin/plexydesk" type="Qnx/Elf" entry="true">bin/plexydesk</asset>
        <asset path="lib">lib</asset>
        <asset path="share">share</asset>
        <asset path="theme">theme</asset>
</qnx>
