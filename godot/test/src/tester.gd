extends Node

enum TestType
{
    Validate,
    Performance,
}


class Result:
    var type: TestType
    var label: String
    var num_tests: int
    var results: Array


class Test:
    var _type: TestType
    var _label: String
    var _setup: Callable = func(): return []
    var _test: Callable = func(): pass
    var _num_tests: int = 1

    func setup(p_fn: Callable) -> Test:
        _setup = p_fn
        return self

    func test(p_fn: Callable) -> Test:
        _test = p_fn
        return self

    func run() -> Result:
        push_error("Not implemented!")
        return null


class PerfTest extends Test:
    func run() -> Result:
        var total := 0.0
        for i in _num_tests:
            var ctx = _setup.call()
            var start_time := Time.get_ticks_msec()
            _test.callv(ctx)
            var diff := float(Time.get_ticks_msec() - start_time) / 1000.0
            total += diff
        var avg := total / _num_tests
        var res := Result.new()
        res.type = TestType.Performance
        res.label = _label
        res.num_tests = _num_tests
        res.results = [total, avg]
        return res

class ValidTest extends Test:
    var _messages: PackedStringArray = []
    var _pass := true

    func run() -> Result:
        _test.callv([self] + _setup.call())
        var res := Result.new()
        res.type = TestType.Validate
        res.label = _label
        res.results = [_pass, _messages]
        return res

    func assert_eq(lhs: Variant, rhs: Variant) -> void:
        if lhs != rhs:
            _pass = false
            _messages.append("Assertion Failed: %s == %s" % [lhs, rhs])

    func assert_eq_approx(lhs: Variant, rhs: Variant) -> void:
        if !is_equal_approx(lhs, rhs):
            _pass = false
            _messages.append("Assertion Failed: is_equal_approx(%s, %s)" % [lhs, rhs])

signal result(res: Result)
signal complete()

var _tests: Array[Test] = []

func perf(p_label: String, p_num_tests: int) -> Test:
    var test := PerfTest.new()
    test._type = TestType.Performance
    test._label = p_label;
    test._num_tests = p_num_tests
    _tests.append(test)
    return test

func valid(p_label: String) -> Test:
    var test := ValidTest.new()
    test._type = TestType.Validate
    test._label = p_label;
    _tests.append(test)
    return test

func run(p_type: TestType) -> void:
    for test in _tests:
        if test._type != p_type:
            continue
        var res := test.run()
        result.emit(res)
    complete.emit()

func define_tests() -> void:
    pass

func run_all() -> void:
    print_rich("[b]Validate[/b]")
    run(TestType.Validate)

    print_rich("\n[b]Performance[/b]")
    run(TestType.Performance)

    print_rich("\n[b][color=green]Done[/color][/b]")


func _handle_result(p_result: Result) -> void:
    if p_result.type == TestType.Performance:
        print_rich("{label} @ {num_tests} : Total={total}, Avg={avg}".format({
            label = p_result.label,
            num_tests = p_result.num_tests,
            total = "%0.2fs" % p_result.results[0],
            avg = "%0.5fs" % p_result.results[1],
        }))

    elif p_result.type == TestType.Validate:
        print_rich("{label} : {pass_fail}".format({
            label = p_result.label,
            pass_fail = "[color=green]PASS[/color]" if p_result.results[0] else "[color=red]FAIL[/color]"
        }))

        if len(p_result.results[1]) > 0:
            for message in p_result.results[1]:
                print_rich("[indent]" + message)

func _init() -> void:
    result.connect(_handle_result)
    define_tests()
    run_all()
