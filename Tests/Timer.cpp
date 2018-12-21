#include <catch.hpp>
#include <kt/Timer.h>


TEST_CASE("Duration comparison tests", "[Timer]")
{
	kt::Duration const oneSecond = kt::Duration::FromSeconds(1.0);
	kt::Duration const oneMillisecond = kt::Duration::FromMilliseconds(1.0);
	kt::Duration const oneMicrosecond = kt::Duration::FromMicroseconds(1.0);

	CHECK(oneSecond > oneMillisecond);
	CHECK(!(oneSecond < oneMillisecond));
	CHECK(!(oneSecond == oneMillisecond));

	CHECK(oneMillisecond > oneMicrosecond);
	CHECK(!(oneMillisecond < oneMicrosecond));
	CHECK(!(oneMillisecond == oneMicrosecond));

	CHECK(oneSecond > oneMicrosecond);
	CHECK(!(oneSecond < oneMicrosecond));
	CHECK(!(oneSecond == oneMicrosecond));

	CHECK(oneSecond == oneSecond);
	CHECK(oneSecond >= oneSecond);
	CHECK(oneSecond <= oneSecond);
	CHECK(!(oneSecond != oneSecond));
	CHECK(!(oneSecond < oneSecond));
	CHECK(!(oneSecond > oneSecond));

	CHECK(oneMillisecond == oneMillisecond);
	CHECK(oneMillisecond >= oneMillisecond);
	CHECK(oneMillisecond <= oneMillisecond);
	CHECK(!(oneMillisecond != oneMillisecond));
	CHECK(!(oneMillisecond < oneMillisecond));
	CHECK(!(oneMillisecond > oneMillisecond));

	CHECK(oneMicrosecond == oneMicrosecond);
	CHECK(oneMicrosecond >= oneMicrosecond);
	CHECK(oneMicrosecond <= oneMicrosecond);
	CHECK(!(oneMicrosecond != oneMicrosecond));
	CHECK(!(oneMicrosecond < oneMicrosecond));
	CHECK(!(oneMicrosecond > oneMicrosecond));

}

TEST_CASE("Duration manipulation", "[Timer]")
{
	kt::Duration const oneSecond = kt::Duration::FromSeconds(1.0);
	kt::Duration const twoSeconds = kt::Duration::FromSeconds(2.0);

	CHECK(twoSeconds - oneSecond == oneSecond);
	CHECK(oneSecond * 2.0 == twoSeconds);
	CHECK(oneSecond + twoSeconds == kt::Duration::FromSeconds(3.0));

	kt::Duration oneSecondMutable = oneSecond;
	oneSecondMutable += oneSecond;
	CHECK(oneSecondMutable == twoSeconds);
	oneSecondMutable -= oneSecond;
	CHECK(oneSecondMutable == oneSecond);
}