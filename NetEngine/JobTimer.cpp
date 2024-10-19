#include "pch.h"
#include "JobTimer.h"
#include "JobQueue.h"

void JobTimer::Reserve(uint64 tickAfter, weak_ptr<JobQueue> owner, JobPtr job)
{
	const uint64 executeTick = ::GetTickCount64() + tickAfter;
	JobData* jobData = new JobData(owner, job);

	EXCLUSIVE_LOCK;

	_items.push(TimerItem{ executeTick, jobData });
}

void JobTimer::Distribute(uint64 now)
{
	// �� ���� 1 �����常 ���
	if (_distributing.exchange(true) == true)
		return;

	vector<TimerItem> items;

	{
		EXCLUSIVE_LOCK;

		while (_items.empty() == false)
		{
			const TimerItem& timerItem = _items.top();
			if (now < timerItem.executeTick)
				break;

			items.push_back(timerItem);
			_items.pop();
		}
	}

	for (TimerItem& item : items)
	{
		if (JobQueuePtr owner = item.jobData->owner.lock())
			owner->Push(item.jobData->job, THREAD_TYPE::LOGIC);

		delete item.jobData;
	}

	// �������� Ǯ���ش�
	_distributing.store(false);
}

void JobTimer::Clear()
{
	EXCLUSIVE_LOCK;

	while (_items.empty() == false)
	{
		const TimerItem& timerItem = _items.top();
		delete timerItem.jobData;
		_items.pop();
	}
}