node('builder'){
    docker.image('riddleandcode/wallet-builder').inside('-u "root"') {
        try {
            checkout scm
            stage('Generating build') {
                sh 'mkdir -p build && cd build && cmake ../'
            }
            stage('Coding Guideline') {
                    sh 'astyle "src/*.c" "inc/*.h" "tests/*.c" "tests/*.h" --style=google -s2'
                    sh 'echo \'if [ $(find . -iname "*.orig" | wc -l) -eq 0 ]; then echo "According to guideline."; else echo "Not according to guideline" && exit 1; fi\' > guide && sh guide'
            }
            dir('build')
            {
                stage('Build') {
                    sh 'make'
                }
                stage('Testing') {
                    sh 'export LSAN_OPTIONS=verbosity=1:log_threads=1 && make check'
                    sh 'xsltproc /opt/ctest/ctest2junix.xsl tests/Testing/$(head -1 tests/Testing/TAG)/Test.xml > CTestResults.xml '
                    junit 'CTestResults.xml'
                    cobertura coberturaReportFile: 'coverage.xml'
                }
            }
        }
        catch (error) {
            echo "The pipeline failed"
            throw error
        }
        finally {
            sh "git clean -xfd"
        }
    }
}
