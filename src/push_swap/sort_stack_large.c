/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   sort_stack_large.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dda-silv <dda-silv@student.42lisboa.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/15 10:22:23 by dda-silv          #+#    #+#             */
/*   Updated: 2021/04/15 12:52:11y dda-silv         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "sort_stack_large.h"

/*
** For larger stacks (5 and more), the logics works as follows:
** 1. Split stack A in 2 - above median in A & below median in B
** 2. Merge stack B into A
**    - First push to A values above 1st quartile and min values
**    - Second, push to A values below 1st quartile
** 3. In a loop, by increments of 12, parse the stack A and sort it using
**    the stack B
** @param:	- [t_list **] head of the list stack_a
**			- [t_list **] head of the list stack_b
** Line-by-line comments:
** @line-line	comment
*/

void	sort_stack_large(t_list **stack_a, t_list **stack_b)
{
	int		len_b;
	t_list	*medians;

	medians = ft_lstnew((void *)(ft_lst_get_max(*stack_a) + 1));
	ft_lstadd_front(&medians, ft_lstnew((void *)ft_lst_get_median(*stack_a)));

	split_a_in_two_w_median(stack_a, stack_b, (long int)medians->data, is_less_than_or_equal);

	len_b = ft_lstsize(*stack_b);
	merge_b_into_a_w_median(stack_a, stack_b, len_b, &medians);

	sort_half_stack_a(stack_a, stack_b, &medians);

	while (is_first_node_a_sorted(*stack_a))
		rotate_stack_print(stack_a, "ra");

	ft_lstadd_front(&medians, ft_lstnew((void *)(ft_lst_get_max(*stack_a) + 1)));
	ft_lstadd_front(&medians, ft_lstnew((void *)ft_lst_get_median(*stack_a)));
	split_a_in_two_w_median(stack_a, stack_b, (long int)medians->data, is_greater_than);

	len_b = ft_lstsize(*stack_b);
	merge_b_into_a_w_median(stack_a, stack_b, len_b, &medians);

	sort_half_stack_a(stack_a, stack_b, &medians);

	rotate_until_sorted(stack_a);
	ft_lstclear(&medians, ft_lstdel_int);
}

static void	sort_half_stack_a(t_list **stack_a,
				t_list **stack_b,
				t_list **medians)
{
	int	ra_count;
	int	min;

	min = ft_lst_get_min(*stack_a);
	while ((*medians)->next)
	{
		while (is_first_node_a_sorted(*stack_a) && (long int)(*stack_a)->data != min)
			rotate_stack_print(stack_a, "ra");
		ra_count = 0;
		while ((*stack_a)->data < (*medians)->data)
		{
			if ((long int)(*stack_a)->data == min)
				break ;
			if ((*stack_a)->data <= (*medians)->data)
				push_stack_print(stack_b, stack_a, "pb");
			else if (++ra_count)
				rotate_stack_print(stack_a, "ra");
		}
		while (ra_count--)
			rev_rotate_stack_print(stack_a, "rra");
		merge_b_into_a_in_order(stack_a, stack_b);
		ft_lstdel_first(medians, ft_lstdel_int);
	}
}

static void	merge_b_into_a_w_median(t_list **stack_a,
				t_list **stack_b,
				int old_len,
				t_list **medians)
{
	int	curr_len;

	if (old_len == 0 || !(*stack_b))
		return ;
	ft_lstadd_front(medians, ft_lstnew((void *)ft_lst_get_median(*stack_b)));
	curr_len = old_len;
	while (curr_len > old_len / 2 && *stack_b)
	{
		if ((long int)(*stack_b)->data == ft_lst_get_min(*stack_b))
		{
			push_stack_print(stack_a, stack_b, "pa");
			if (*stack_b != 0)
				rotate_stack_print(stack_a, "ra");
		}
		else if ((*medians)->data <= (*stack_b)->data)
		{
			curr_len--;
			push_stack_print(stack_a, stack_b, "pa");
		}
		else
			rotate_stack_print(stack_b, "rb");
	}
	merge_b_into_a_w_median(stack_a, stack_b, curr_len, medians);
}

// static int	get_stack_increment(t_list *medians, t_list *stack_a)
// {
// 	t_list	*dup;
// 	int		index_first;
// 	int		index_second;
// 	int		stack_increment;

// 	dup = ft_lstdup(stack_a);
// 	ft_lst_sort(&dup, ascending);
// 	index_first = ft_lst_get_node_index(dup, (long int)medians->data);
// 	index_second = ft_lst_get_node_index(dup, (long int)medians->next->data);
// 	if (index_second - index_first >= MAX_STACK_INCREMENT)
// 		stack_increment = (long int)ft_lst_get_data_node(dup, index_first + MAX_STACK_INCREMENT);
// 	else
// 		stack_increment = (long int)medians->data;
// 	ft_lstclear(&dup, ft_lstdel_int);
// 	return (stack_increment);
// }

// static void	add_medians(t_list *medians, t_list *stack_a)
// {
// 	t_list	*dup;
// 	int		index_first;
// 	int		index_second;
// 	void	*data_intermediate_node;

// 	dup = ft_lstdup(stack_a);
// 	ft_lst_sort(&dup, ascending);
// 	while (medians->next)
// 	{
// 		index_first = ft_lst_get_node_index(dup, (long int)medians->data);
// 		index_second = ft_lst_get_node_index(dup, (long int)medians->next->data);
// 		if (index_second - index_first > STACK_INCREMENT)
// 		{
// 			data_intermediate_node = ft_lst_get_data_node(dup, index_first +
// 				(index_second - index_first) / 2);
// 			ft_lstadd_after_another(medians, ft_lstnew(data_intermediate_node));
// 		}
// 		medians = medians->next;
// 	}
// }

static int	is_first_node_a_sorted(t_list *stack_a)
{
	t_list	*dup;
	int		index_first_node;
	int		index_last_node;
	void	*data_last_node;

	data_last_node = ft_lst_get_data_last_node(stack_a);
	dup = ft_lstdup(stack_a);
	ft_lst_sort(&dup, ascending);
	index_first_node = ft_lst_get_node_index(dup, (long long)stack_a->data);
	index_last_node = ft_lst_get_node_index(dup, (long long)data_last_node);
	ft_lstclear(&dup, ft_lstdel_int);
	if (index_last_node == (index_first_node - 1))
		return (1);
	else
		return (0);
}

static void	merge_b_into_a_in_order(t_list **stack_a, t_list **stack_b)
{
	int	min_b;
	int	max_b;

	min_b = ft_lst_get_min(*stack_b);
	max_b = ft_lst_get_max(*stack_b);
	while (*stack_b)
	{
		if ((long int)(*stack_b)->data == ft_lst_get_max(*stack_b))
			push_stack_print(stack_a, stack_b, "pa");
		else if ((long int)(*stack_b)->data == ft_lst_get_min(*stack_b))
		{
			push_stack_print(stack_a, stack_b, "pa");
			rotate_stack_print(stack_a, "ra");
		}
		else if (is_min_max_closer_to_top(*stack_b, min_b, max_b))
			rotate_stack_print(stack_b, "rb");
		else
			rev_rotate_stack_print(stack_b, "rrb");
	}
}

/*
** Check if min or max of stack_b is closer to top than it is from bottom
** @param:	- [t_list *] stack b
**			- [int] min of stack b
** @return:	[int] true or false
** Line-by-line comments:
** @9-10	If index min is closer to top than max then to know if either
*/

static int	is_min_max_closer_to_top(t_list *stack, int min, int max)
{
	int		len;
	int		index_min;
	int		index_max;
	int		check;

	len = ft_lstsize(stack);
	index_min = ft_lst_get_node_index(stack, min);
	index_max = ft_lst_get_node_index(stack, max);
	if (index_min < index_max)
		check = (index_min < (len - index_max));
	else
		check = (index_max < (len - index_min));
	return (check);
}
